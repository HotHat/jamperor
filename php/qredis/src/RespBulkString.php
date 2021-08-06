<?php declare(strict_types=1);


namespace Resp;


class RespBulkString extends RespString
{
   private $isNull;

   public function __construct($val)
   {
       parent::__construct($val);
       $this->isNull = false;
   }

    public function null() {
       $this->isNull = true;
   }

   public function toString()
   {
       if ($this->isNull) { return "$-1\r\n"; }

       return '$' . strlen($this->val) . "\r\n" . $this->val . "\r\n";
   }
}