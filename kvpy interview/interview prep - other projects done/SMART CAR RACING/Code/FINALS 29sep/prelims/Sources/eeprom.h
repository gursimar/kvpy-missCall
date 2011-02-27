#define ERR_OK           0             /* OK */
#define ERR_SPEED        1             /* This device does not work in the active speed mode. */
#define ERR_RANGE        2             /* Parameter out of range. */
#define ERR_VALUE        3             /* Parameter of incorrect value. */
#define ERR_OVERFLOW     4             /* Timer overflow. */
#define ERR_MATH         5             /* Overflow during evaluation. */
#define ERR_ENABLED      6             /* Device is enabled. */
#define ERR_DISABLED     7             /* Device is disabled. */
#define ERR_BUSY         8             /* Device is busy. */
#define ERR_NOTAVAIL     9             /* Requested value or method not available. */
#define ERR_RXEMPTY      10            /* No data in receiver. */
#define ERR_TXFULL       11            /* Transmitter is full. */
#define ERR_BUSOFF       12            /* Bus not available. */
#define ERR_OVERRUN      13            /* Overrun error is detected. */
#define ERR_FRAMING      14            /* Framing error is detected. */
#define ERR_PARITY       15            /* Parity error is detected. */
#define ERR_NOISE        16            /* Noise error is detected. */
#define ERR_IDLE         17            /* Idle error is detected. */
#define ERR_FAULT        18            /* Fault error is detected. */
#define ERR_BREAK        19            /* Break char is received during communication. */
#define ERR_CRC          20            /* CRC error is detected. */
#define ERR_ARBITR       21            /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define ERR_PROTECT      22            /* Protection error is detected. */
#define ERR_UNDERFLOW    23            /* Underflow error is detected. */
#define ERR_UNDERRUN     24            /* Underrun error is detected. */
#define ERR_COMMON       25            /* Common error of a device. */
#define ERR_LINSYNC      26            /* LIN synchronization error is detected. */


/* User type for addressing of the EEPROM. Actual type depends on the CPU family. */
  typedef far unsigned int * far IEE1_TAddress; /* Type of address to the EEPROM */

/* EEPROM area start address (in the format used by bean methods). */
#define IEE1_AREA_START                 ((IEE1_TAddress)1306624UL)
/* EEPROM area last address (in the format used by bean methods). */
#define IEE1_AREA_END                   ((IEE1_TAddress)1310715UL)
/* EEPROM area size (in bytes). */
#define IEE1_AREA_SIZE                  4092U
/* EEPROM area sector size: minimal erasable unit (in bytes). */
#define IEE1_AREA_SECTOR_SIZE           4U
/* TRUE if the bean setting allows clear bits of already programmed flash memory location without destruction of the value in surrounding addresses by sector erase. */
#define IEE1_ALLOW_CLEAR                (TRUE)
/* Size of programming phrase, i.e. number of bytes that must be programmed at once */
#define IEE1_PROGRAMMING_PHRASE         (2U)


/*
** ===================================================================
**     Method      :  WriteWord (bean IntEEPROM)
**
**     Description :
**         The method writes the word data to EEPROM memory.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static byte WriteWord(IEE1_TAddress AddrRow,word Data16)
{
  if (ESTAT_CBEIF == 0) {              /* Is command buffer full ? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* ESTAT: PVIOL=1,ACCERR=1 */
  ESTAT = 48;                          /* Clear error flags */
  *(AddrRow) = Data16;                 /* Array address and program data */
  /* ECMD: ??=0,CMDB=32 */
  ECMD = 32;                           /* Word program command */
  ESTAT = 128;                         /* Clear flag command buffer empty */
  if ((ESTAT_PVIOL == 1) || (ESTAT_ACCERR == 1)) { /* Is protection violation or acces error detected ? */
    return ERR_NOTAVAIL;               /* If yes then error */
  }
  while ((ESTAT & 0xC0) != 0xC0) {}    /* Wait for command completition */
  if (*(AddrRow) != Data16) {          /* Was attempt to write data to the given address errorneous? */
    return ERR_VALUE;                  /* If yes then error */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  WriteSector (bean IntEEPROM)
**
**     Description :
**         The method writes the data block to EEPROM memory.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static byte WriteSector(IEE1_TAddress AddrSec,dword Data32)
{
  byte err;                            /* Temporary variable */

  if (ESTAT_CBEIF == 0) {              /* Is command buffer full ? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* ESTAT: PVIOL=1,ACCERR=1 */
  ESTAT = 48;                          /* Clear error flags */
  *(AddrSec) = (word)(Data32 >> 16);   /* Array address and program data - higher part */
  /* ECMD: ??=0,CMDB=64 */
  ECMD = 64;                           /* Sector erase command */
  ESTAT = 128;                         /* Clear flag command buffer empty */
  if ((ESTAT_PVIOL == 1) || (ESTAT_ACCERR == 1)) { /* Is protection violation or acces error detected ? */
    return ERR_NOTAVAIL;               /* If yes then error */
  }
  while ((ESTAT & 0xC0) != 0xC0) {}    /* Wait for command completition */
  ESTAT = 48;                          /* Clear error flags */
  *(AddrSec) = (word)(Data32 >> 16);   /* Array address and program data - higher part */
  /* ECMD: ??=0,CMDB=32 */
  ECMD = 32;                           /* Word program command */
  ESTAT = 128;                         /* Clear flag command buffer empty */
  if ((ESTAT_PVIOL == 1) || (ESTAT_ACCERR == 1)) { /* Is protection violation or acces error detected ? */
    return ERR_NOTAVAIL;               /* If yes then error */
  }
  while ((ESTAT & 0xC0) != 0xC0) {}    /* If yes then wait for command completition */
  err=WriteWord(AddrSec + 1,(word)Data32); /* Write lower part */
  if (err != ERR_OK) {                 /* Was attemp to write data to the given address errorneous? */
    return err;                        /* If yes then error */
  }
  if (*(AddrSec) != (word)(Data32 >> 16)) { /* Was attempt to write data to the given address errorneous? */
    return ERR_VALUE;                  /* If yes then error */
  }
  return ERR_OK;
}

byte IEE1_SetLong(IEE1_TAddress Addr,dword Data)
{
  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if ((dword)Addr & 3) {               /* Aligned address ? */
    return ERR_NOTAVAIL;
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  return (WriteSector(Addr,Data));     /* Write new content of given sector */
}

/*
** ===================================================================
**     Method      :  IEE1_GetLong (bean IntEEPROM)
**
**     Description :
**         This method reads a long word (4 bytes) from the specified
**         EEPROM address. The method also sets address pointer for
**         <SetActByte> and <GetActByte> methods (applicable only if
**         these methods are enabled). The pointer is set to address
**         passed as the parameter + 3.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to EEPROM
**       * Data            - A pointer to the returned 32-bit data
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_BUSY - device is busy 
**                           - ERR_RANGE - selected address out of the
**                           selected address range
** ===================================================================
*/

byte IEE1_GetLong(IEE1_TAddress Addr,dword *Data)
{
  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  *Data = *(far dword *far)(Addr);     /* Return data from given address */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IEE1_SetByte (bean IntEEPROM)
**
**     Description :
**         This method writes a given byte to a specified address in
**         EEPROM. The method also sets address pointer for <SetActByte>
**         and <GetActByte> methods (applicable only if these methods
**         are enabled). The pointer is set to address passed as the
**         parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to EEPROM
**         Data            - Data to write
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_SPEED - the bean does not work in the
**                           active speed mode 
**                           - ERR_BUSY - device is busy 
**                           - ERR_VALUE - verification of written data
**                           failed (read value does not match with
**                           written value) 
**                           - ERR_NOTAVAIL - other device-specific
**                           error 
**                           - ERR_RANGE - parameter Addr is out of range
** ===================================================================
*/

byte IEE1_SetByte(IEE1_TAddress Addr,byte Data)
{
  union {
    byte b[4];
    word w[2];
    long l;
  } backup;
  byte idx;
  IEE1_TAddress SecAddr;               /* EEPROM Sector address */

  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  SecAddr = (IEE1_TAddress)((dword)Addr & 16777212); /* Aligned sector address */
  backup.w[0] = *(SecAddr);            /* Load sector to variable backup */
  backup.w[1] = *(SecAddr+1);
  backup.b[(dword)(Addr) & 3] = Data;  /* Store data to variable backup */
  if (*(Addr) == 0xFFFF) {             /* Is the word erased? */
    SecAddr = (IEE1_TAddress)((dword)Addr & 16777214); /* Aligned word address */
    idx = (byte) (((dword)(Addr) & 0x0002) >> 1); /* Word index in sector */
    return (WriteWord(SecAddr,backup.w[idx])); /* Write new content */
  }
  else {                               /* Is given address non-erased ? */
    return (WriteSector(SecAddr,(dword)backup.l)); /* If yes then write new content */
  }
}

/*
** ===================================================================
**     Method      :  IEE1_GetByte (bean IntEEPROM)
**
**     Description :
**         This method reads a byte from a specified EEPROM address.
**         The method also sets address pointer for <SetActByte> and
**         <GetActByte> methods (applicable only if these methods are
**         enabled). The pointer is set to address passed as the
**         parameter.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - EEPROM Address
**       * Data            - A pointer to the returned 8-bit data
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_BUSY - device is busy 
**                           - ERR_RANGE - parameter Addr is out of range
** ===================================================================
*/

byte IEE1_GetByte(IEE1_TAddress Addr,byte *Data)
{
  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  *Data = *((byte *far)Addr);
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IEE1_SetWord (bean IntEEPROM)
**
**     Description :
**         This method writes a given word (2 bytes) to the specified
**         address in EEPROM. The method also sets address pointer for
**         <SetActByte> and <GetActByte> methods (applicable only if
**         these methods are enabled). The pointer is set to address
**         passed as the parameter + 1.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to EEPROM
**         Data            - Data to write
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_SPEED - the bean does not work in the
**                           active speed mode 
**                           - ERR_BUSY - device is busy 
**                           - ERR_VALUE - verification of written data
**                           failed (read value does not match with
**                           written value) 
**                           - ERR_NOTAVAIL - other device-specific
**                           error 
**                           - ERR_RANGE - selected address out of the
**                           selected address range
** ===================================================================
*/
byte IEE1_SetWord(IEE1_TAddress Addr,word Data)
{
  IEE1_TAddress SecAddr;               /* EEPROM Sector address */

  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if ((dword)Addr & 1) {               /* Aligned address ? */
    return ERR_NOTAVAIL;
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  if (*(Addr) == 65535) {              /* Is given EEPROM row erased ? */
    return (WriteWord(Addr,Data));     /* Write new content */
  }
  else {                               /* Is given address non-erased ? */
    if ((dword)Addr & 2) {             /* Is given address from high part of the sector ? */
      SecAddr = (IEE1_TAddress)((dword)Addr & 16777212); /* aligned sector address */
      return (WriteSector(SecAddr,((dword)Data | ((dword)(*(SecAddr)) << 16))));
    } else {                           /* Is given address from low part of the sector ? */
      return (WriteSector(Addr,(((dword)Data << 16) | (*(Addr+1)))));
    }
  }
}

/*
** ===================================================================
**     Method      :  IEE1_GetWord (bean IntEEPROM)
**
**     Description :
**         This method reads a word (2 bytes) from the specified EEPROM
**         address. The method also sets address pointer for
**         <SetActByte> and <GetActByte> methods (applicable only if
**         these methods are enabled). The pointer is set to address
**         passed as the parameter + 1.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to EEPROM
**       * Data            - Pointer to returned 16-bit data
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_BUSY - device is busy 
**                           - ERR_RANGE - selected address out of the
**                           selected address range
** ===================================================================
*/
byte IEE1_GetWord(IEE1_TAddress Addr,word *Data)
{
  if((Addr < IEE1_AREA_START) || (Addr > IEE1_AREA_END)) { /* Is given address out of EEPROM area array ? */
    return ERR_RANGE;                  /* If yes then error */
  }
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  *Data = *Addr;                       /* Return data from given address */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IEE1_EraseEeprom (bean IntEEPROM)
**
**     Description :
**         The method mass erases all EEPROM memory. The method also
**         sets address pointer for <SetActByte> and <GetActByte>
**         methods (applicable only if these methods are enabled). The
**         pointer is set to address passed as the parameter - 1.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Pass any valid EEPROM address. EEPROM
**                           module in some CPU families don't require
**                           address for mass erase, then the parameter
**                           is ignored.
**     Returns     :
**         ---             - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_SPEED - the bean does not work in the
**                           active speed mode 
**                           - ERR_BUSY - device is busy 
**                           - ERR_NOTAVAIL - other device-specific error
** ===================================================================
*/
byte IEE1_EraseEeprom(IEE1_TAddress Addr)
{
  if((ESTAT & 192) != 192) {           /* Is reading from EEPROM possible? */
    return ERR_BUSY;                   /* If no then error */
  }
  /* ESTAT: PVIOL=1,ACCERR=1 */
  ESTAT = 48;                          /* Clear error flags */
  *((IEE1_TAddress)Addr) = 0;          /* Array address */
  /* ECMD: ??=0,CMDB=64 */
  ECMD = 65;                           /* Mass erase command */
  ESTAT = 128;                         /* Clear flag command buffer empty */
  if ((ESTAT_PVIOL == 1) || (ESTAT_ACCERR == 1)) { /* Is protection violation or acces error detected ? */
    return ERR_NOTAVAIL;               /* If yes then error */
  }
  while ((ESTAT & 0xC0) != 0xC0) {}    /* Wait for command completition */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  IEE1_Init (bean IntEEPROM)
**
**     Description :
**         Initializes the associated peripheral(s) and the bean internal 
**         variables. The method is called automatically as a part of the 
**         application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void EEPROM_Init(void)
{
            ECLKDIV = 74;                        /* Set up Clock Divider Register */
}

void EEPROM_masserase()
{
          unsigned char return_val;
          return_val = IEE1_EraseEeprom(IEE1_AREA_START);
          if(return_val == ERR_OK) 
          {
                    //Sucessfully erased..
          }
}

void EEPROM_read()
{
          /*
          reads kp
          reads ki
          reads kd
          reads setpoint
          reads delay_dc
          reads start          
          */

          unsigned char return_val;
          unsigned char* read_pointer_char;
          unsigned int* read_pointer_int;
          
          // To read kp from EEPROM
          read_pointer_char = &kp ;
          return_val=IEE1_GetByte(EEPROM_KP,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }

          // To read ki from EEPROM
          read_pointer_char = &ki ;
          return_val=IEE1_GetByte(EEPROM_KI,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }

          // To read kd from EEPROM
          read_pointer_char = &kd ;
          return_val=IEE1_GetByte(EEPROM_KD,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }

          // To read DELAY_DC from EEPROM
          read_pointer_char = &delay_dc ;
          return_val=IEE1_GetByte(EEPROM_DELAYDC,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in delay_dc
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          
          // To read SETPOINT from EEPROM         unsigned int
          read_pointer_int = &upper ;
          return_val=IEE1_GetWord(EEPROM_SETPOINT,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          
    /*      
//BLACK VALUES
          read_pointer_char = &b_val[0] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL0,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[1] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL1,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[2] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL2,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[3] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL3,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[4] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL4,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[5] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL5,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &b_val[6] ;
          return_val=IEE1_GetByte(EEPROM_B_VAL6,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          
          
//WHITE VAL
          read_pointer_char = &w_val[0] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL0,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[1] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL1,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[2] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL2,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[3] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL3,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[4] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL4,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[5] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL5,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_char = &w_val[6] ;
          return_val=IEE1_GetByte(EEPROM_W_VAL6,read_pointer_char);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          
//COM_RATIOS          
          read_pointer_int = &com_ratio[0] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO0,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[1] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO1,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[2] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO2,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[3] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO3,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[4] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO4,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[5] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO5,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          read_pointer_int = &com_ratio[6] ;
          return_val=IEE1_GetWord(EEPROM_COM_RATIO6,read_pointer_int);
          if(return_val == ERR_OK) 
          {
                    //do nothing...
                    //already got the value in setpoint
          }
          else
          {
                    //ERROR_LED=LED_ON
          }
          
       */   
          
}
 
void EEPROM_reset()
{
          //to add the code for sensor compensation and set it to 0..
          unsigned char return_val;
          unsigned char* read_pointer_char;

          return_val = IEE1_EraseEeprom(IEE1_AREA_START);
          if(return_val == ERR_OK) 
          {
                    LED3=LED_ON;
                    Delay(2);
                    LED3=LED_OFF;
                    Delay(2);
          }

          // for kp
          return_val = IEE1_SetByte(EEPROM_KP, EEPROM_KP_DEFAULT);
          if(return_val == ERR_OK) 
          {                      
                    read_pointer_char = &kp ;
                    return_val=IEE1_GetByte(EEPROM_KP,read_pointer_char);
                    if(return_val == ERR_OK) 
                    {
                              //do nothing...
                              //already got the value in setpoint
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);

                    }
                    else
                    {
                              //ERROR_LED=LED_ON
                    }
          }          

          // for ki
          return_val = IEE1_SetByte(EEPROM_KI, EEPROM_KI_DEFAULT);
          if(return_val == ERR_OK) 
          {                      
                    read_pointer_char = &ki ;
                    return_val=IEE1_GetByte(EEPROM_KI,read_pointer_char);
                    if(return_val == ERR_OK) 
                    {
                              //do nothing...
                              //already got the value in setpoint
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);

                    }
                    else
                    {
                              //ERROR_LED=LED_ON
                    }
          }          

          // for kd
          return_val = IEE1_SetByte(EEPROM_KD, EEPROM_KD_DEFAULT);
          if(return_val == ERR_OK) 
          {                      
                    read_pointer_char = &kd ;
                    return_val=IEE1_GetByte(EEPROM_KD,read_pointer_char);
                    if(return_val == ERR_OK) 
                    {
                              //do nothing...
                              //already got the value in setpoint
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);

                    }
                    else
                    {
                              //ERROR_LED=LED_ON
                    }
          }          

          
          // for set point                    setpoint is unsigned int...
          return_val = IEE1_SetWord(EEPROM_SETPOINT,EEPROM_SETPOINT_DEFAULT);
          if(return_val == ERR_OK) 
          {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
          }          
          
          //for delay_dc
          return_val = IEE1_SetByte(EEPROM_DELAYDC,EEPROM_DELAYDC_DEFAULT);
          if(return_val == ERR_OK) 
          {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
          }          
          
          
          //WRITE TO EEPROM..           THE CORRENT VALUES OF SENSORS....
                    return_val = IEE1_SetByte(EEPROM_B_VAL0,b_val[0]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL1,b_val[1]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL2,b_val[2]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL3,b_val[3]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL4,b_val[4]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL5,b_val[5]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_B_VAL6,b_val[6]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    
                    //WHITE VALUES
                    return_val = IEE1_SetByte(EEPROM_W_VAL0,w_val[0]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL1,w_val[1]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL2,w_val[2]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL3,w_val[3]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL4,w_val[4]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL5,w_val[5]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetByte(EEPROM_W_VAL6,w_val[6]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    
                    // COM-RATIO
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO0,com_ratio[0]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO1,com_ratio[1]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO2,com_ratio[2]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO3,com_ratio[3]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO4,com_ratio[4]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO5,com_ratio[5]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          
                    return_val = IEE1_SetWord(EEPROM_COM_RATIO6,com_ratio[6]);
                    if(return_val == ERR_OK) 
                    {                      
                              LED3=LED_ON;
                              Delay(2);
                              LED3=LED_OFF;
                              Delay(2);
                    }          


}
