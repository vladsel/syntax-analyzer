PROGRAM PARSERTEST7 ;
	BEGIN
		LOOP 
			LOOP
				LOOP
					CASE FIRST OF
					ENDCASE;
				ENDLOOP 			(*error*)
			endloop; 				(*error*)
		ENDLOOP;
	END.