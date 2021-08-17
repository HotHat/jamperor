<?php declare(strict_types=1);


namespace Resp;


class RespArray extends RespBase
{
    private $chunk;
    private $isNull;


    public function __construct()
    {
        $this->chunk = [];
        $this->isNull = false;
    }

    public function add($resp) {
        $this->chunk[] = $resp;
    }

    public function null() {
        $this->isNull = true;
    }

    public function toString() {
        if ($this->isNull) { return "*-1\r\n"; }

        $message = '*' . count($this->chunk) . "\r\n";
        foreach ($this->chunk as $it) {
            $message .= $it->toString();
        }

        return $message;
    }
}