	  if(tick_motor){
		  tick_motor=0; 		  
		  //Motor routine
		  seq_index=motor_run(&dir,&counter,&max); //Get motor index sequence
		  for(i=0;i<4;i++)
			  Bits1_PutBit(0,sequence[seq_index][i]);
	  } 
	  
	  if(tick_sensors){
		  tick_sensors=0; z
		  
		  if(counter_measures<max_measures)
			  counter_measures++;
		  else
			  counter_measures = 0;
		  
		  //Lidar
		  AD1_MeasureChan(0,0);		  		  
			
		  //Sonar
		  
		  //trigger
		  Bit1_PutVal(1);
		  FC81_Reset();
		  while(sonar_UStimer<15){
			do
				err=FC81_GetTimeUS(&sonar_UStimer);
			while(err!=ERR_OK);
		  }
		  sonar_UStimer=0;
		  Bit1_PutVal(0);		  
		  //Capture		
		  Cap1_Reset();	  
		  
	  }	  
	  //Comunication		  
	
	  if(is_send){
		  //Make packet
		  make_packet(counter,sonar_value,lidar_value,&data);
		  //Send data
		  do
			  err=AS1_SendBlock((byte*) &data,sizeof(data),&Sent);
		  while(err!=ERR_OK);
	  }
	
	  /*
	  extern unsigned short sonar_measure;
	  void Cap1_OnCapture(void){	
	  	do
	  	  err=Cap1_GetCaptureValue(&sonar_measure);
	  	while(err!=ERR_OK);
	  }*/
	 