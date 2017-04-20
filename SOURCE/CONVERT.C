typedef unsigned char byte;
void BitConvert(byte data,byte *badres,byte number ,byte strike)
{
     __asm
     {
         push ds
         lds di,badres
         mov al,data
         cmp strike,0
         mov cx,0
         jz et6
         cmp strike,1
         jz et2
         cmp strike,2
         jz e4
         mov cl,4
         jmp e3
  e4:  mov cl,4
       sub cl,number
       jmp e3
 et2:   mov cl,8
        sub cl,number
 e3:   
         shr al,cl
         mov cl,0
  et6: cmp number,0
         jz et5
  et1: shr al,1
         jnb et3
         mov ds:[di],0xff
         jmp et4
  et3: mov ds:[di],0x00
  et4: inc cl
        inc di
        cmp cl,number
        jne et1
  et5:pop ds
  }
}

byte ByteConvert(byte *badres,byte number ,byte strike)
{ 
   byte value=0;
   __asm
   {
      les di,badres
      mov cx,0
      cmp number,0
      jz end
et1:mov al,es:[di]
      rol al,1
      rcr value,1
      inc cx
      inc di
      cmp cl,number
      jne et1
      cmp strike,0
      jnz end
      cmp number,8
      je end
      mov cx,4
et2:shr value,1  
      dec cl
      cmp cl,0
      jne et2         
end:
    }
    return value;
}
    
