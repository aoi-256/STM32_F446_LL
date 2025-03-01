uint8_t I2C_Mem_Read1(uint8_t I2C_ADDR, uint8_t REG_ADDR, uint8_t* Read_Buffer){

	  uint8_t Error_Count = 0;

	  //ACK Enable
	  I2C1->CR1 |= 0x0400;

	  //Generate Start
	  I2C1->CR1 |= 0x0100;

	  //Check Start
	  while(!(I2C1->SR1 & 0x01)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 1;
		  }
	  }
	  Error_Count = 0;

	  //Send ADDR(Write)
	  I2C1->DR = I2C_ADDR << 1;

	  //Check ADDR
	  while(!(I2C1->SR1 & 0x02)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 2;
		  }
	  }
	  Error_Count = 0;

	  //Clear ADDR Flag
	  (void)I2C1->SR2;

	  //Send MemADDR
	  I2C1->DR = REG_ADDR;

	  //Check TXE
	  while(!(I2C1->SR1 & 0x80)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 3;
		  }
	  }
	  Error_Count = 0;

	  //----------------------------------------------------//

	  //Restart
	  I2C1->CR1 |= 0x0100;

	  //Check Start
	  while(!(I2C1->SR1 & 0x01)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 4;
		  }
	  }
	  Error_Count = 0;

	  //Send ADDR(Read)
	  I2C1->DR = (I2C_ADDR << 1) | 0x01;

	  //Check ADDR
	  while(!(I2C1->SR1 & 0x02)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 5;
		  }
	  }
	  Error_Count = 0;

	  //Clear ADDR Flag
	  (void)I2C1->SR2;

	  //ACK Disable
	  I2C1->CR1 &= ~0x0400;

	  //Wait RXNE
	  while(!(I2C1->SR1 & 0x40)){

		  Error_Count ++;
		  if(Error_Count > 1000){

			  return 6;
		  }
	  }
	  Error_Count = 0;

	  //Read Data
	  Read_Buffer[0] = I2C1->DR;

	  //Generate Stop
	  I2C1->CR1 |= 0x0200;

	  return 0;
}
