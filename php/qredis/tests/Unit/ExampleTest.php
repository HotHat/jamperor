<?php declare(strict_types=1);

namespace Tests\Unit;

use PHPUnit\Framework\TestCase;
use Resp\Client;
use Resp\RespArray;
use Resp\RespBulkString;
use Resp\RespInteger;
use Resp\RespString;
use Resp\StreamConnection;


class ExampleTest extends TestCase {

    public function testRespString() {
        $a = new RespString("abc");
        $this->assertEquals($a->toString(), "+abc\r\n");

        $b = new RespBulkString("abc");
        $this->assertEquals($b->toString(), "$3\r\nabc\r\n");

        $c = new RespBulkString("abc");
        $c->null();
        $this->assertEquals($c->toString(), "$-1\r\n");

        $d = new RespInteger("918273987");
        $this->assertEquals($d->toString(), ":918273987\r\n");

        $e = new RespArray();
        $e->add($a);
        $e->add($b);
        $e->add($c);
        $e->add($d);

        echo $e->toString();

    }

    public function testRespArray() {

    }


    public function testExample()
    {

        $connect = new StreamConnection('127.0.0.1', 6379);

        $e = new RespArray();
        $e->add(new RespBulkString("get3"));
        $e->add(new RespBulkString("888abc"));
        // $e->add(new RespBulkString("444444"));

        $connect->write($e);

        $r = $connect->read();

        echo $r->toString();
    }
}