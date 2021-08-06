<?php declare(strict_types=1);


namespace Resp;


class StreamConnection
{


    private $resource;
    private $host;
    private $port;


    public function __construct($host, $port)
    {
        $this->host = $host;
        $this->port = $port;
    }

    public function read() {
        $socket = $this->getResource();
        $chunk = fgets($socket);

        if ($chunk === false || $chunk === '') {
            throw new RespException('Error while reading line from the server.');
        }

        $prefix = $chunk[0];
        $payload = substr($chunk, 1, -2);

        switch ($prefix) {
            case '+':
                return new RespString($payload);

            case '$':
                $size = (int) $payload;

                if ($size === -1) {
                    $bulk = new RespBulkString("");
                    $bulk->null();
                    return $bulk;
                }

                $bulkData = '';
                $bytesLeft = ($size += 2);

                do {
                    $chunk = fread($socket, min($bytesLeft, 4096));

                    if ($chunk === false || $chunk === '') {
                        $this->onConnectionError('Error while reading bytes from the server.');
                    }

                    $bulkData .= $chunk;
                    $bytesLeft = $size - strlen($bulkData);
                } while ($bytesLeft > 0);

                return new RespBulkString( substr($bulkData, 0, -2));

            case '*':
                $count = (int) $payload;

                $multiBulk = new RespArray();

                if ($count === -1) {
                    $multiBulk->null();
                    return $multiBulk;
                }


                for ($i = 0; $i < $count; ++$i) {
                    $multiBulk->add($this->read());
                }

                return $multiBulk;

            case ':':
                $integer = (int) $payload;
                return new RespInteger($integer == $payload ? $integer : $payload);

            case '-':
                return new RespError($payload);

            default:
                $this->onConnectionError(new RespException("Unknown response prefix: '$prefix'."));

        }
    }

    public function write(RespArray $resp) {
        $socket = $this->getResource();

        $buffer = $resp->toString();

        while (($length = strlen($buffer)) > 0) {
            $written = @fwrite($socket, $buffer);

            if ($length === $written) {
                return;
            }

            if ($written === false || $written === 0) {
                $this->onConnectionError(new RespException('Error while writing bytes to the server.'));
            }

            $buffer = substr($buffer, $written);
        }
    }

    public function getResource()
    {
        if (isset($this->resource)) {
            return $this->resource;
        }

        $this->connect();

        return $this->resource;
    }

    public function connect() {
        $this->resource = $this->createStreamSocket($this->host, $this->port);
    }

    public function disconnect() {
        if ($this->isConnected()) {
            fclose($this->resource);
        }
    }

    public function __destruct()
    {
        $this->disconnect();
    }


    public function isConnected() {
        return isset($this->resource);
    }

    private function createStreamSocket($host, $port) {
        $timeout = 5.0;
        $flags = STREAM_CLIENT_CONNECT;
        $resource = stream_socket_client(sprintf("tcp://%s:%s", $host, $port), $errno, $errstr, $timeout, $flags);

        if (!$resource) {
            $this->onConnectionError(new RespException($errstr, $errno));
        }

        return $resource;
    }

    private function onConnectionError($exception) {

        if ($this->isConnected()) {
            $this->disconnect();
        }

        throw $exception;
    }
}