/*
 * calculator.h
 *
 *  Created on: Sep 23, 2023
 *  Author: omar
 */




/*
 * FEATURES :
 * -	basic mathematical operations ( + , - , * , / ) between integer numbers
 * -	basic mathematical operations ( + , - , * , / ) between decimal numbers till two numbers after the decimal point
 * -	the result of any operation can be used in another one 
 * -	the system can be resetted by enter double dot 
 * -	( * , / ) can not be enter at the first of operation or after any operation such as ( +* , -* , +/ , -/ ) 
 * -	Any consecutive (+) and (-) or (-) and (-) turn into (-)
 *
 * */
#include "StdTypes.h"
#include "Delay.h"
#include "Dio.h"
#include "Lcd.h"
#include "Keypad.h"
#include "calculator.h"



	//ARRAY OF FALGS FOR THE 16 KEYPAD BUTTONS
    u8 buttonFlags[16] = {0};
	u8 Display_counter = 0 , Operation_Counter = 0 , Pre_Operation = 0 , Last_Operation = 0, Operation_Flag = 0 , Pre_Keypad_Counter = 0 , Dot_Counter = 0 , Pre_Dot = 0 , EQ = 0 , DIV_Flag = 0 , Program_Counter = 0 , INVALID = 0 , ERROR = 0;
	s32 Total_Entered_Number1 = 0 , Total_Entered_Number2 = 0, Result1 = 0 ,Result2 = 0 , ANS1 = 0 , ANS2 = 0 , MULL = 1 , DIVV = 1 , MIN = 0;

void Run_Calculator(void)
{

    Keypad_ButtonType Keypad_Counter;

    Lcd_Init(&Lcd_Configuration);
    Keypad_Init();
    while (1)
    {
    	/*
    	 * READ THE PRESSED BUTTON FROM THE KEYPAD
    	 * */
        for (Keypad_Counter = KEYPAD_B00; Keypad_Counter <= KEYPAD_B15; Keypad_Counter++)
        {
        	/*
        	 * CHECK WHAT IS THE PRESSED BUTTON
        	 * */
            if(Keypad_GetButtonState(Keypad_Counter) == KEYPAD_PRESSED)
            {   

                if (buttonFlags[Keypad_Counter] == 0)
                {   

                	 Program_Counter++;
                   /*
                    * IF THE PRESSED BUTTON FROM 0 - 9
                    * */
					if (Keypad_Counter <= 9)
					{

						/*
						 * CLEAR THE DISPLAY AFTER ANY OPERATION TO DO ANOTHER ONE
						 * WITHOUT USING THE PRE RESULT
						 * */
						if (Operation_Counter > 1 && Operation_Flag == 0)
						{
							Reset_Calc();
							Program_Counter = 1;
						}

					/*
					 * DISPLAY THE BRESSED NUMBER FROM 0 TO 9
					 * */
                    Lcd_DisplayNumber(Keypad_Counter);


                    /*
                    * FOR EX :
    				* IF THE ENTERED NUMBER = 23
    				* SO, 2 WILL BE ENTERED FIRST AND WILL BE
    				* STORED IN Total_Entered_Number
    				* THEN WHEN 3 WILL BE ENTERED 2 MUST BE MULTIPLIED BY 10
    				* TO BE 20 AND ADD TO 3
    				* SO THE RESULT WILL BE 23
    				*
    				*** NOTE THAT THIS RULES FOR NATURAL AND DECIMAL NUMBERS ***
    				*/
                    if (Pre_Dot == 1)
                    {
                    	Total_Entered_Number2 = Keypad_Counter + (Total_Entered_Number2 * 10 ) ;
                    }
                    else
                    {
                    	Total_Entered_Number1 = Keypad_Counter + (Total_Entered_Number1 * 10 ) ;
                    }


                	Pre_Operation = 0;
				} // END OF => if (Keypad_Counter <= 9)

                	/*
                	 * THE BUTTONS TO DISPLAY THE OPERATORS (+ , - , * , / , = )
                	 * */
					else if ( Keypad_Counter > 9 )
					{

	                     /*
	                      * CALLING THE Calc_Get_Operation FUNCTION TO DISPLAY THE OPERATOR SYMPOL
	                      * IN CASE THE USER DID NOT ENTER ANY INVALID SYMPOL AND THE BEGIN OF THE EQUATION ( / , * )
	                      * */
                   	     Calc_Get_Operation (Keypad_Counter) ;


                        if(Keypad_Counter == DOT)
                        {
    						Dot_Counter++;
                        }

						/*
						 * IF THE USER ENTER THE DOT BUTTON TWICE , THE CALCULATOR WILL BE RESETED
						 * */
						if (Keypad_Counter == DOT && Pre_Dot == 1 )
							{
							   Reset_Calc();
							}


						/*
						 * IF THE ENTERED OPERATION DOSE NOT THE DOT TWICE
						 * SO THE OPERATION WILL BE CALCULATED
						 *
						 *
						 *** THE OPERATIONS ELSE ***
						 * */
						else
						{

							Operation_Flag = 1 ;

						 /*
						  * WHEN THE FIRST PARAMETER OF ANY NEW OPERATION BE ENTERED
						  * */
	                     if (Operation_Counter == 0)
	                     {

                            //    MULL = Total_Entered_Number1*10;
		                    	Result1 = Total_Entered_Number1 ;
		                    	ANS1 = Result1 ;

		                    	Total_Entered_Number1 = 0 ;
	                     }

	                     /*
	                      * IF THE OPERATION OCCURES BETWEEN DECIMAL NUMBERS
	                      * */
	                     else if (Operation_Counter == 1)
	                     {

                            //    MULL += Total_Entered_Number2 ;

								Result2 = Total_Entered_Number2 ;
		                    	ANS2 = Result2 ;

		                    	Total_Entered_Number2 = 0 ;

	                     }
	                    else
	                     {
	                    	/*
	                    	 * IF THE USER WANT TO USE THE PRE RESULT IN ANOTHER OPERATION
	                    	 * THE WORD "ANS" WILL BE PRINTED ON LCD WHEN THE USER ENTER
	                    	 * ANT OPERATOR
	                    	 * */
	                    	 if (Operation_Counter > 0 && Pre_Operation == EQUAL)
	                    	 {
	                    		 Result1 = ANS1 ;
	                    		 Result2 = ANS2 ;

	                    		 Lcd_ClearDisplay ();
		                    	 Lcd_DisplayString ("ANS");
		                   	     Calc_Get_Operation (Keypad_Counter) ;
		                    	 /*
		                    	  * RETURN THE DISPLAY COUNTER TO 3 AND THAT IS THE NUMBER OF THE
		                    	  * WORD "ANS" TO AVOID THE DISPALY SHIFT ON LCD AFTER SOME OPERATIONS
		                    	  * */
		                    	 Display_counter = 3;
	                    	 }
	                     }
                    	 Operation_Counter++;



                        /*
                         * THE FOUR BACIS OPERATION ( + , - , * , / )
                         *
                         * NOTE THAT THIS PROJECT PROVIDES THIS OPERATIONS BETWEEN INTEGER AND DECIMAL VALUE (UP TO TWO NUMBERS AFTER THE DECIMAL POINT )
                         * */

//==========================================================================================================================================
						/*
						 * ++++++++++++++PLUS OPERAION++++++++++++++
						 * */
						if (Last_Operation == PLUS && Keypad_Counter != DOT)
						{
							/*
							 * CALCULATE THE NATURAL NUMBER
							 * */
							if (Pre_Dot == 0 && Dot_Counter == 0)
							{
							Result1 +=  Total_Entered_Number1 ;
							Total_Entered_Number1 = 0 ;
							}

							/*
							 * CALCULATE THE DECIMAL NUMBER
							 * */
							else if (Dot_Counter > 0)
							{

								if (Result2 < 10)
								{
									Result2 *= 10;
								}

								if (Total_Entered_Number2 < 10)
								{
									Total_Entered_Number2 *= 10;
								}

							    Result1 +=  Total_Entered_Number1 ;
							    Total_Entered_Number1 = 0 ;
							    Result2 +=  Total_Entered_Number2 ;
							    Total_Entered_Number2 = 0 ;
							}

                            /*
                             * IF THE TOTAL ENTERED NUMBERED AFTER THE DECIMAL POINT GREATER THAN 100
                             * WE HAVE TO INCREACE RESULR ONE TO MAKE THE FINAL RESULT IS CORRECR
                             *
                             * NOTE THAT I LIMITED THE NUMBER AFTER DECIMAL POINT TO ONLY TWO NUMBER
                             * BUT WE CAN INCREADE THAT AS WE CAN BY THE SAME CONCEPT
                             *
                             * EX : 9.85 + 7.65
                             *  - RESULT1 = 16 , RESULT2 = 150 , TOTAL PRINTED RESULT ON LCD = 16.150 BUT THIS IS WRONG ANSWER
                             *  - AFTER THE FOLLWING EQUATION , RESULT1 = 17 , RESULT2 = 50 , TOTAL PRINTED RESULT ON LCD = 17.50
                             *    AND THIS IS THE CORRECT ANSWER
                             *
                             * */
							if (Result2 > 100)
							{
								Result1 = Result1  + (Result2/100);
								Result2 = Result2 % 100;
							}

						   // Last_Operation = 0;
						} // END OF THE PLUS OPERATION
//==========================================================================================================================================


//==========================================================================================================================================
						/*
						 * ------------MINUS OPERAION------------
						 * */
						else if (Last_Operation == MINUS && Keypad_Counter != DOT)
						{
							/*
							 * CALCULATE THE NATURAL NUMBER
							 * */
						  if (Pre_Dot == 0 && Dot_Counter == 0)
							{
							Result1 -=  Total_Entered_Number1 ;
							Total_Entered_Number1 = 0 ;
							}

							/*
							 * CALCULATE THE DECIMAL NUMBER AND THE FINAL RESULT
							 *
							 * THIS CODE COVER THE SOLUTIONS OF ALL THE TEST CASES
							 * FOR TEST :
							 *   => 5.2  - 4.1  = 1.1
							 *   => 2.51 - 5.32 = -2.81
							 *   => 2.60 - 3.12 = -0.52
							 *   => 3.17 - 2.20 =  0.97
							 *   => 1.17 - 2.20 = -1.03
							 * */
						 else if (Dot_Counter > 0)
						  {
							if (Result2 < 10)
							{
								Result2 *= 10;
							}

							if (Total_Entered_Number2 < 10)
							{
								Total_Entered_Number2 *= 10;
							}

							Result1 -=  Total_Entered_Number1 ;
							Total_Entered_Number1 = 0 ;

							Result2 -=  Total_Entered_Number2 ;
							Total_Entered_Number2 = 0 ;

                            if (Result1 > 0)
                            {
                            	if (Result2 < 0)
                            	{
                            		Result2 = (Result2) * (-1);
                            		MIN = (Result1*100) - Result2;
                            	    Result1 = MIN / 100;
                            	    Result2 = MIN % 100;
                            	}
                            }
                            else if (Result1 < 0)
                            {
                            	if (Result2 > 0)
                            	{
                            	Result2 = 100 - Result2 ;
                            	Result1++;
                            	  if (Result1 == 0)
                            	  {
                            		  Lcd_DisplayCharacter('-');
                            	  }
                            	}
                            	else
                            	{
                            		Result2 *= -1;
                            	}
                             }
                            /*
                             * IF THE NATURAL NUMBERS EQUAL TO ZEROS
                             * WE HAVE TO CHECK THE RESULT OF DECIMAL SUBSTRACTION
                             * IF THE RESULT > 0 , DO NOT CHANGE ANY THING OF THE OUTPUT
                             * IF THE RESULT < 0 , WE MUST CHANGE THE MINUS RESULT TO POSITIVE BU MUL BY -1
                             * AND DISPLAY THE RESULT IN FRONT OF THE NUMBER
                             *
                            */
                            else
                            {
                            	if (Result2 < 0)
                            	{
                            		Result2 *= -1;
        						    Lcd_DisplayCharacter('-');
                            	}
                            }

						 } // END OF THE DECIMAL PART IN MINUS OPERATION

						// Last_Operation = 0;

				   } // END OF THE MINUS OPERATION
//==========================================================================================================================================


//==========================================================================================================================================
						/*
						 * ************MUL OPERAION************
						 * */
						else if (Last_Operation == MUL && Keypad_Counter != DOT)
						{

                           /*
                            * IF THE THE TWO INPUT NUMBERS ARE INTEGERS
                            * */
                           if (Pre_Dot == 0 && Dot_Counter == 0)
                           {
                               MULL = (Result1*(100) + Result2) * Total_Entered_Number1 +  (Result1*(10) + Result2) * Total_Entered_Number2;

                               Result2 = MULL % 100 ;
                               Result1 = MULL/100;
                           }
                           /*
                            * IF THERE ARE AT LEAST ONE DECIMAL NUMBER
                            * */
                           else if (Dot_Counter > 0)
                           {

                        	   if (Result1 == 0 && Total_Entered_Number1 == 0)
                        	   {
                        		   Result1 = 0;
                                   Result2 *= Total_Entered_Number2;
                        	   }
                        	   else
                        	   {

                        		   if (Result2 < 10)
                        		   {
                        			   Result2 *= 10;
                        		   }
                        		   if (Total_Entered_Number2 < 10)
                        		   {
                        			   Total_Entered_Number2 *=10;
                        		   }

                                   MULL = ( (Result1*(100) + Result2) * Total_Entered_Number1*100 ) + (  (Result1*(100) + Result2) * Total_Entered_Number2 );

                                   Result2 = MULL % 10000 ;
                                   Result1 = MULL/10000;

                                   if (Result2 > 100)
                                   {
                                	   Result2 /= 100;
                                   }
                        	   }
                           }

						   Total_Entered_Number1 = 0 ;
						   Total_Entered_Number2 = 0 ;

							Last_Operation = 0;

						}

//==========================================================================================================================================


//==========================================================================================================================================
						/*
						 * /////////////DIV OPERAION////////////
						 * */
						else if (Last_Operation == DIV && Keypad_Counter != DOT)
						{
                      /*
						   if (Keypad_Counter == MINUS)
						   {
							   Result1 *= -1 ;
						   }
				      */
                           /*
                            * IF THE THE TWO INPUT NUMBERS ARE INTEGERS
                            * */
                           if (Pre_Dot == 0 && Dot_Counter == 0)
                           {
                        	      if (Total_Entered_Number1 == 0)
                        	      {
           				    	   Reset_Calc();
           				    	   Lcd_DisplayString ("ERROR");
           				    	   _delay_ms(1200);
           				    	   Lcd_ClearDisplay ();
           				    	   ERROR = 1;
           				    	   INVALID = 1;
                        	      }
                        	      else
                        	      {
                               	      Result1 = (Result1*100) / Total_Entered_Number1;

                                      Result2 = Result1 % 100 ;
                                      Result1 = Result1/100;
                        	      }
                           }
                           /*
                            * IF THERE ARE AT LEAST ONE DECIMAL NUMBER
                            * */
                           else if (Dot_Counter > 0)
                           {
                        	   if (Total_Entered_Number1 == 0 && Total_Entered_Number2 == 0)
                        	   {
        				    	   Reset_Calc();
        				    	   Lcd_DisplayString ("ERROR");
        				    	   _delay_ms(1200);
        				    	   Lcd_ClearDisplay ();
        				    	   ERROR = 1;
           				    	   INVALID = 1;
                        	   }
                               /*
                                * CASE 1 :
                                *  - IF THE NATURAL PART OF THE TWO NUMBER IS ZERO
                                *  - FIRST IF BOTH THE DECIMAL PARTS < 10 OR > 100
                                *  - SECOND IF ONLY ONE OF THEM IS > 10
                                * */
                           else if (Result1 == 0 && Total_Entered_Number1 == 0)
                        	   {
                                  if  ( (Result2 < 10 && Total_Entered_Number2 < 10 )
                                	  || (Result2 < 100 && Result2 > 10 && Total_Entered_Number2 < 100 && Total_Entered_Number2 > 10  ) )
                        		   {
                               	      Result2 = (Result2*100) / Total_Entered_Number2;

                                      Result1 = Result2 / 100;
                                      Result2 = Result2 % 100;
                                      if (Result2 < 10)
                                      {
              							DIV_Flag = 1;
                                      }

                        		   }
                                  else if (Result2 > 10 || Total_Entered_Number2 > 10 )
                       		       {
                       			       // 0.9  / 0.45
                               	     if ( (Result2*10) / Total_Entered_Number2 > Result2 )
                               	     {
                               	    	 Result2 = (Result2*10) / Total_Entered_Number2;
                                         Result1 = Result2 / 100;
                                         Result2 = Result2 % 100;
                               	     }
                               	     else
                               	     {
                               	    	 Result2 = (Result2*10) / Total_Entered_Number2;
                                         Result1 = Result2 % 100;
                                         Result2 = Result2 / 100;
                               	     }

                       		       }
                        	   }
                        	   /*
                        	    * CASE 2 :
                        	    *  - IF THE NATURAL PART OF THE FIRST NUMBER IS EQUAL TO ZERO
                        	    * */
                    		   else if (Result1 == 0 )
                    		   {
                    				   Result2 = Result2*1000 / (Total_Entered_Number1*100 + Total_Entered_Number2);
                    		   }
                        	   /*
                        	    * CASE 3 :
                        	    *  - IF THE DECIMAL PART OF THE TWO NUMBERS IS ZERO (AS INTEGER )
                        	    *  - IF BOTH OF THE DECIMAL PARTS IS < 10
                        	    *  - IF ONLY ONE OF THE DECIMAL PARTS OF THE TWO NUMBERS IS > 10
                        	    *  - IF IF BOTH OF THE DECIMAL PARTS IS > 100
                        	    * */
                        	   else
                        	   {
                        		   if (Result2 == 0 && Total_Entered_Number2 == 0)
                        		   {
                                	   Result1 = (Result1*100) / Total_Entered_Number1;

                                       Result2 = Result1 % 100 ;
                                       Result1 = Result1/100;
                        		   }
                        		   else
                        		   {
                        			 if (Result2 < 10 && Total_Entered_Number2 < 10)
                        			 {
                                         DIVV = (Result1*1000) / (Total_Entered_Number1*10 + Total_Entered_Number2 ) + (Result2*100) / ((Total_Entered_Number1*10 + Total_Entered_Number2 ));

                                         Result1 = DIVV / 100;
                                         Result2 = DIVV % 100;
                        			 }
                        			 else if (Result2 > 10 || Total_Entered_Number2 > 10)
                        			 {
                        				 if (Result2 < Total_Entered_Number2)
                        				 {
                                             DIVV = (Result1*10000) / (Total_Entered_Number1*100 + Total_Entered_Number2 ) + (Result2*1000) / ((Total_Entered_Number1*100 + Total_Entered_Number2 ));

                                             Result1 = DIVV / 100;
                                             Result2 = DIVV % 100;
                        				 }
                        				 else if (Result2 > Total_Entered_Number2)
                        				 {
                                             DIVV = (Result1*10000) / (Total_Entered_Number1*100 + Total_Entered_Number2 ) + (Result2*100) / ((Total_Entered_Number1*100 + Total_Entered_Number2 ));

                                             Result1 = DIVV / 100;
                                             Result2 = DIVV % 100;
                        				 }
                        			 }
                        		  }
                        	   }
                           }
						   Total_Entered_Number1 = 0 ;
						   Total_Entered_Number2 = 0 ;

							Last_Operation = 0;
						}
//==========================================================================================================================================


//==========================================================================================================================================



						/*
						 * OPERATION FLAGS
						 * */
						if (Keypad_Counter == DOT)
						{
							Pre_Dot = 1;
						}
						else if (Keypad_Counter == EQUAL)
						{
							EQ = EQUAL ;
							Pre_Dot = 0;
						}
						else
						{
							Last_Operation = Keypad_Counter;
							Pre_Dot = 0;
						}


					       if ( (Pre_Operation == MUL || Pre_Operation == DIV)
					    	   && (Keypad_Counter == MINUS || Keypad_Counter == PLUS) )
			                {
					    	   /*
					    	    * 5*-5   , 4/+3   THAT IS VALID BUT NOT VICE VERSA
					    	    * 5-*5   , 4+/3   THAT IS INVALID
					    	    * */
					    		   Last_Operation = Pre_Operation;
			                }

                           /*
                            * TWO CONSECUTIVE MINUS = PLUS
                            * */
					       if (Keypad_Counter == MINUS && Pre_Operation == MINUS)
					       {
					    	   if(Program_Counter != 1)
					    	   {
					    		   Last_Operation = PLUS;
					    	   }
					       }

                           /*
                            * TWO CONSECUTIVE MINUS AND PLUS OR PLUS AND MINUS = MINUS
                            * */
					       if ( (Keypad_Counter == MINUS && Pre_Operation == PLUS)
					    	   || (Keypad_Counter == PLUS && Pre_Operation == MINUS) )
					       {
					    	   if(Program_Counter != 1)
					    	   {
					    		   Last_Operation = MINUS;
					    	   }
					       }


						/*
                        * IF THE PRESSED BUTTON IS EQUAL , THE FINAL RESULT OF ANY
                        * OPERATION WILL BE DISPLAYED IN THE DECIMAL FORMAT
                        *
                        * AND THE VARIABLES Result1 , Result2 WILL BE RESETED TO ZERO TO ENTER IN A NEW OPERATION
                        * */
						if (Keypad_Counter == EQUAL)
						{
							/*
							 * DISPLAT THE RESULT OF ANY OPERATION
							 * EXPECT IF THERE ARE A MATH ERROR SUCH AS DIVISION BY ZERO
							 * */
							if (ERROR != 1)
							{
								Lcd_DisplayNumber(Result1);
							    Lcd_DisplayCharacter('.');
							    if(DIV_Flag == 1)
							    {
									Lcd_DisplayNumber(0);
									DIV_Flag = 0;
							    }
								Lcd_DisplayNumber(Result2);
							}

							ERROR = 0;
							INVALID = 0;

							ANS1 = Result1 ;
							ANS2 = Result2 ;

							Result1 = 0 ;
							Result2 = 0 ;

							MULL = 1;
							DIVV = 1;

							Operation_Flag = 0 ;
					    	Program_Counter = 0;
						}


	                       /*
	                        * IT IS NOT VALID TO ENTER ( / , * ) IN THE BEGIN OF THE EQUATION
	                        * BUT VALID TO ENTER ( + , - )
	                        * */
					       if ( (Keypad_Counter == MUL && Program_Counter == 1)
					    	   ||  (Keypad_Counter == DIV && Program_Counter == 1) )
					       {
					    	   Reset_Calc();
					    	   Lcd_DisplayString ("INVALID INPUT");
					    	   _delay_ms(1200);
					    	   Lcd_ClearDisplay ();
					    	   Program_Counter = 0;
       				    	   INVALID = 1;
					       }

					       /*
					        * IT IS INVALID TO ENTER TWO CONSECUTIVE ( * , / ) IN ANY EQUATION
					        * */
					       if ( ( (Keypad_Counter == MUL || Keypad_Counter == DIV) && (Pre_Operation == PLUS || Pre_Operation == MINUS) ) )
			                {
					    	   Reset_Calc();
					    	   Lcd_DisplayString ("INVALID INPUT");
					    	   _delay_ms(1200);
					    	   Lcd_ClearDisplay ();
					    	   Program_Counter = 0;
       				    	   INVALID = 1;
			                }

					       if (Pre_Operation == MUL || Pre_Operation == DIV)
			                {
								/*
								 * IF THE USER ENTER TWO OR MORE CONSECUTIVELY MUL OR DIV OPERATOR
								 * THAT IS INVALID
								 * AND THE CALCULATOR WILL BE RESETED
								 * */
					    	   if (Keypad_Counter == MUL || Keypad_Counter == DIV)
					    	   {
						    	   Reset_Calc();
						    	   Lcd_DisplayString ("INVALID INPUT");
						    	   _delay_ms(1200);
						    	   Lcd_ClearDisplay ();
						    	   Program_Counter = 0;
           				    	   INVALID = 1;
					    	   }

					    	   /*
					    	    * 5*-5   , 4/+3   THAT IS VALID BUT NOT VICE VERSA
					    	    * 5-*5   , 4+/3   THAT IS INVALID
					    	    * */
					  	      if (Keypad_Counter == MINUS )
					  	      {
					    		   Total_Entered_Number1 = -1 ;
					    		   if (Dot_Counter > 0)
					    		   {
						    		   Total_Entered_Number2 = 1 ;
					    		   }
					  	      }

					  	      if (Keypad_Counter == PLUS)
					    	   {
					    		   Total_Entered_Number1 = 1 ;
					    		   if (Dot_Counter > 0)
					    		   {
						    		   Total_Entered_Number2 = 1 ;
					    		   }
					    	   }
			                }

                           /*
                            * THE LAST SYMBOL THAT THE USER ENTER
                            * */
					       if (INVALID != 1)
					       {
								Pre_Operation = Keypad_Counter;
					       }
					       else
					       {
					    	   INVALID = 0;
					       }


					} // END OF THE OPERATIONS ELSE

			        /*
			         * SET THE FALG OF THE PRESSED BUTTON TO AVOID DISPLAY IT
			         * ON LCD MORE THAN ONE IN EACH PRESS
			         * */

					} // END OF THE CONDITION else if ( Keypad_Counter > 9 )

					buttonFlags[Keypad_Counter] = 1;
					
					/*
					 * IF THE BUTTON PRESSED , THE DISPLAY COUNTER WILL INCREASE BY 1
					 * AND IF THE DISPLAY COUNTER > 16 , THE DISPLAY WILL SHIFTED LEFT BY
					 * 1 CHARACTER FOR EACH TIME
					 * */
					Display_counter++;
					if (Display_counter > 16 )
					{
						Lcd_ShiftDisplayLeft(1);
					}

					// Program_Counter++;
				 } // END OF THE CONDITION if (buttonFlags[Keypad_Counter] == 0)

               }//4 - END OF KEY PRESSED CHECK
            else
           {
            buttonFlags[Keypad_Counter] = 0;
           }

      } //3 - END OF THE FOR LOOP
  } //2 - END OF THE WHILE LOOP
}  //1 - END OF THE FUNCTION



void Reset_Calc (void)
{
	 Lcd_ClearDisplay ();

	 Total_Entered_Number1 = 0 ;
	 Total_Entered_Number2 = 0 ;

	 Operation_Counter = 0;
	 Display_counter = 0;

	 Operation_Flag = 0 ;
	 Pre_Operation = 0;

	 Last_Operation = 0;
	 Dot_Counter = 0;


	 Result1 = 0 ;
	 Result2 = 0 ;

	 MULL = 1;
	 DIVV = 1;

	 ANS1 = 0 ;
	 ANS2 = 0 ;

	 Pre_Dot = 0;
}

void Calc_Get_Operation (u8 operation)
{
     switch (operation)
	 {
         case PLUS :
           	Lcd_DisplayCharacter ('+');
			break;
		case MINUS :
           	Lcd_DisplayCharacter ('-');
			break;
		case MUL :
           	Lcd_DisplayCharacter ('*');
			break;
		case DIV :
           	Lcd_DisplayCharacter ('/');
			break;
		case EQUAL :
           	Lcd_DisplayCharacter ('=');
			break;
		case DOT :
		    Lcd_DisplayCharacter ('.');
		    break;
	}
	
}
