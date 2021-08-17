<?php declare(strict_types=1);


namespace Resp;


class RespError extends RespString
{
    public function toString()
    {
        return '-' . $this->val . "\r\n";
    }

}