<?php declare(strict_types=1);


namespace Resp;


class RespString extends RespBase
{
    protected $val;

    public function __construct($val)
    {
        $this->val = $val;
    }

    public function toString() {
        return '+' . $this->val . "\r\n";
    }
}