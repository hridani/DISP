             
             
 NMCMOD mod[MAXNUMMOD]; 	//����� �� ������
unsigned int BaudRate; 
//---------------------------------------------------------------------------//
//  ��� �� ���������:  NmcHardReset                                             //
//  ������� ��������:   0=Fail, 1=Success                                        //
//  ���������:     addr: ����� �� ������ (1-32) ��� ������ ����� (0x80-0xFF) //
//  Description:   Reset �� ������ ���������� � ������ ����� 'addr'         //
//---------------------------------------------------------------------------// 
BOOL NmcHardReset(byte addr)
{
   				int i;
				char cstr[5];

//����� �� 0 �� ����������� �� �������� �����
			cstr[0] = 0;
			for (i=0; i<20; i++) 
						comport<<cstr

		//     reset=komanden string
			cstr[0] = 0xAA;			//��������
			cstr[1] = addr;
			cstr[2] = HARD_RESET;
			cstr[3] = (byte)(cstr[1] + cstr[2]);  //checksum

                 comport<<cstr

				sleep(100);		//wait for reset to execute

			for (i=0; i<nummod; i++) delete mod[i].p;  //��������� �� ������������ ������
			nummod = 0;
			SioError = 0;	//�������� �� ���� ������
           comport.SetSpeed(19200);//�������� �� ������� �� ������������

			return TRUE;
}           
    
//---------------------------------------------------------------------------//
//  ��� �� ���������:  NmcChangeBaud                                            //
//  ������� ��������:   0=���������, 1=�����                                       //
//  Parameters:     groupaddr: ������ ����� (0x80 - 0xFF)                   //
//                  baudrate: 19200, 57600, 115200, 230400 (PIC-SERVO only)  //
//  Description: ������� �� ��������� �� ������ ���������� � ������ �����       //
//                   'groupaddr'  � ���� �� �������� .  ��� �� ������ �� � ����� �������� � ����� 'groupaddr'.                                  //
//---------------------------------------------------------------------------//
BOOL NmcChangeBaud(byte groupaddr, unsigned int baudrate)
{
		char cstr[6];

		cstr[0] = 0xAA;			//Header
		cstr[1] = groupaddr;
		cstr[2] = 0x10 | SET_BAUD;
		
		switch (baudrate)
		 {
				case 19200:	
					cstr[3] =  b; 
					break;
			case 57600:		
					cstr[3] = PB57600; 
					break;
			case 115200:	
					cstr[3] = PB115200; 
					break;
			case 230400:	
					cstr[3] = PB230400; 
					break;
    		default:	
    		//������ � �� ������� �� ������������ ��������
    			cstr[3] = PB19200;
                baudrate = 19200;
      }
	  cstr[4] = (byte)(cstr[1] + cstr[2] + cstr[3]);  //checksum
      comport<<cstr ;//��������� �� �������

      // ? Sleep(100);		//������ �� �� �� ���� ������������ �� ���������

	   comport.SetSpeed(baudrate)     ; //�������� �� ���� ������� � ��� UART          
	   //���������� �� �������� ������� �� ������ �� UART
         //����� �� �� ������
      BaudRate = baudrate;
     return true;
}    
    
//---------------------------------------------------------------------------//
//  ��� �� ���������:  InitVars                   //
//  Return Value:   ����                                                     //
//  ���������:    ����                                                  //
// ��������:    I������������� �� �����.                       //
//---------------------------------------------------------------------------//
 void InitVars(void)
{
			int i;
          	mod[0].modver = 0;
			mod[0].stat = 0;
			mod[0].statusitems = 0;
			mod[0].groupaddr = 0xFF;
			mod[0].groupleader = false;

			for (i=1; i<MAXNUMMOD; i++)
			{
				 mod[i].stat = 0;
   				 mod[i].statusitems = 0;
   				 mod[i].groupaddr = 0xFF;
    			 mod[i].groupleader = false;
    		}
}       
                 
              
//---------------------------------------------------------------------------//
//  ��� �� ���������:  NmcInit                                                  //
//  ������� ��������:   Returns the number of controller found on network        //
// ���������:     portname: ��� ��  COM port ("COMn:", where n=1-2)        //
//                  baudrate: 19200, 57600, 115200,   //
//  ��������: �������������� �� ������������ � ������� � ��������� ����� , �������� �� 1    //
//----------------------------------------------------------------------
int NmcInit(char *portname, unsigned int baudrate)
{
			DWORD numread;
			byte addr;
			char cstr[20];
			SERVOMOD *p;
			//char mess[40];

			BaudRate = baudrate;

			InitVars();

			serial comport(portname, 19200, NO_PARITY, 8, 1);  //������� �� ������������ 19200 
		    if (comport==NULL) 
		    	return 0;

			NmcHardReset(0xFF);

			for (addr = 1; addr < MAXNUMMOD; addr++)
			{
 			   //����� �� ����������� ����� � �������� ��������
   				 cstr[0] = 0xAA;		//�������� ����
   				 cstr[1] = 0;  		//������� �� � ����������� ����� 0
   				 cstr[2] = 0x20 | SET_ADDR;     //������� ������������ �� �����
   				 cstr[3] = addr;  	//�������� �����
    			 cstr[4] = 0xFF;		//������������ �� ������ �����
    			 cstr[5] = (byte)(cstr[1] + cstr[2] + cstr[3] + cstr[4]);  //checksum

                 comport<<cstr;     //��������� �� �������
              //  numread = SioGetChars(ComPort, cstr, 2);  //������ ������   
                     comport>>cstr;   //������ ������  
    if (numread!=2) break;	//if no response, punt out of loop
    if (cstr[0] != cstr[1])
    	{
        ErrorMsgBox("Status checksum error - please reset the Network");
        SioClose(ComPort);
        return(0);
        }

    //���������� �� ������ ������ - ��� ������ ����� �� �� ������ ���� ����� �������� �������� �����
    cstr[0] = 0xAA;		//�������� ����
    cstr[1] = addr;  		//�����
    cstr[2] = 0x10 | READ_STAT;     //   ������ �� ���� �����
    cstr[3] = SEND_ID;  //���� ������ � ���
    cstr[4] = (byte)(cstr[1] + cstr[2] + cstr[3]);  //checksum
   
    comport<<cstr;     //��������� �� �������

   // numread = SioGetChars(ComPort, cstr, 4);  //get back stat, type, ver, cksum
    if (numread!=4)
    	{
        ErrorMsgBox("Could not read device type");
        SioClose(ComPort);
        return(0);
        }
    mod[addr].stat = cstr[0];
    mod[addr].modtype = cstr[1];
    mod[addr].modver = cstr[2];
    mod[addr].statusitems = 0;
    mod[addr].groupaddr = 0xFF;
    mod[addr].groupleader = false;
    switch (mod[addr].modtype)
    	{
        case SERVOMODTYPE: 	mod[addr].p = ServoNewMod();
        					//fix for change in I/O CTRL command
        					if ((mod[addr].modver<10) || (mod[addr].modver>20))
                              {
        					  p = (SERVOMOD *)(mod[addr].p);
                              p->ioctrl = IO1_IN | IO2_IN;
                              }
        					break;
        case IOMODTYPE: 	mod[addr].p = IoNewMod(); break;
        case STEPMODTYPE: 	mod[addr].p = StepNewMod(); break;
        }

    }

nummod = addr-1;

if (nummod > 0)
	{
    NmcChangeBaud(0xFF, baudrate);
	//if (nummod>1) sprintf(mess,"%d Modules found", nummod);
    //else sprintf(mess,"%d Module found", nummod);
	//ErrorMsgBox(mess);
    }
else
	{
    //ErrorMsgBox("No modules found on the network.\nPlease check power and connections.");
    SioClose(ComPort);
    }

return(nummod);	//Return number of modules
}                 




/---------------------------------------------------------------------------//
//  ��� �� ���������:  ServoNewMod                   //
//  ������� ��������:   �������� ��� ������ SERVOMOD ���������                   //
//  ���������:     ����                                                   //
//  ��������:    ��������� � ������������� �� ������  SERVOMOD ���������.        //
//---------------------------------------------------------------------------//
SERVOMOD *ServoNewMod()
{
		SERVOMOD *p;

		p = new SERVOMOD;
		p->pos = 0;
		p->ad = 0;
		p->vel = 0;
		p->aux = 0;
		p->home = 0;
		p->perror = 0;
		p->cmdpos = 0;
		p->cmdvel = 0;
		p->cmdacc = 0;
		p->cmdpwm = 0;

		(p->gain).kp = 0;
		(p->gain).kd = 0;
		(p->gain).ki = 0;
		(p->gain).il = 0;
		(p->gain).ol = 0;
		(p->gain).cl = 0;
		(p->gain).el = 0;
		(p->gain).sr = 1;
		(p->gain).dc = 0;
		(p->gain).sm = 1;

		p->stoppos = 0;
		p->ioctrl = 0;
		p->homectrl = 0;
		p->movectrl = 0;
		p->stopctrl = 0;
		return p;
}


      
      
//---------------------------------------------------------------------------//
//  ��� �� ���������  ServoGetStat             //
//  ������� ��������:   0=������, 1=�����                                        //
//  ���������:     addr:����� �� ����� (1-32)                              //
//  Description:    ��������� � ����������� �� ��������  PIC-SERVO ����� �����.     //
//---------------------------------------------------------------------------//
BOOL ServoGetStat(byte addr)
{
		int numbytes, numrcvd;
		int i, bytecount;
		byte cksum;
		byte inbuf[20];
		SERVOMOD *p;
		char msgstr[80];
	
		p = (SERVOMOD *)(mod[addr].p);  //�������� ��� �����

		//�������
		numbytes = 2;       //������� �� ���  stat & cksum
		if ( (mod[addr].statusitems) & SEND_POS )
				numbytes +=4;
		if ( (mod[addr].statusitems) & SEND_AD )
			 	numbytes +=1;
		if ( (mod[addr].statusitems) & SEND_VEL ) 
				numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_AUX ) 
				numbytes +=1;
		if ( (mod[addr].statusitems) & SEND_HOME )	
				numbytes +=4;
		if ( (mod[addr].statusitems) & SEND_ID ) 
				numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_PERROR )
				numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_NPOINTS )
				 numbytes +=1;
		numrcvd = SioGetChars(ComPort, (char *)inbuf, numbytes);   ??

//Verify enough data was read
		if (numrcvd != numbytes)
	{
    sprintf(msgstr,"ServoGetStat (%d) failed to read chars",addr);
    ErrorMsgBox(msgstr);
    return false;
    }

//Verify checksum:
cksum = 0;
for (i=0; i<numbytes-1; i++) cksum = (byte)(cksum + inbuf[i]);
if (cksum != inbuf[numbytes-1])
	{
    sprintf(msgstr,"ServoGetStat(%d): checksum error",addr);
    ErrorMsgBox(msgstr);
    return false;
    }

//Verify command was received intact before updating status data
mod[addr].stat = inbuf[0];
if (mod[addr].stat & CKSUM_ERROR)
	{
    ErrorMsgBox("Command checksum error!");
    return false;
    }

//Finally, fill in status data
bytecount = 1;
if ( (mod[addr].statusitems) & SEND_POS )
	{
	p->pos = *( (long *)(inbuf + bytecount) );
    bytecount +=4;
    }
if ( (mod[addr].statusitems) & SEND_AD )
	{
    p->ad = inbuf[bytecount];
    bytecount +=1;
    }
if ( (mod[addr].statusitems) & SEND_VEL )
	{
	p->vel = *( (short int *)(inbuf + bytecount) );
    bytecount +=2;
    }
if ( (mod[addr].statusitems) & SEND_AUX )
	{
    p->aux = inbuf[bytecount];
    bytecount +=1;
    }
if ( (mod[addr].statusitems) & SEND_HOME )
	{
    p->home = *( (unsigned long *)(inbuf + bytecount) );
    bytecount +=4;
    }
if ( (mod[addr].statusitems) & SEND_ID )
	{
    mod[addr].modtype = inbuf[bytecount];
    mod[addr].modver = inbuf[bytecount+1];
    bytecount +=2;
    }
if ( (mod[addr].statusitems) & SEND_PERROR )
	{
	p->perror = *( (short int *)(inbuf + bytecount) );
    bytecount +=2;
    }
if ( (mod[addr].statusitems) & SEND_NPOINTS )
	{
    p->npoints = inbuf[bytecount];
    //bytecount +=1;
    }

return TRUE;
}

      