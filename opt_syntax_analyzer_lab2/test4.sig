PROGRAM PARSERTEST4 ;
	BEGIN
		LOOP 
			LOOP
				LOOP
					LOOP
						LOOP 12.5 			(*error*)
							CASE FIRST OF
							ENDCASE;
						ENDLOOP
					ENDLOOP;
				endloop; 					(*error*)
			ENDLOOP;
		ENDLOOP;
	END.

PROGRAM parserTEST44 ;
    BEGIN
    	CASE 2 OF
		ENDCASE ;
		CASE 3 OF
		ENDCASE ;
		CASE 4 OF
		ENDCASE ;
		(************ COMMENT ************)
		LOOP
    END.