PROGRAM PARSERTEST9;
	BEGIN 
		CASE COUNT OF COUNT * 50:
			/
				CASE COUNT1 OF COUNT1 / 25: 
				/
					case COUNT2 OF 222 MOD COUNT2:  		(*must be error*)
					/
						CASE COUNT3 OF ENDCASE;
					\
					ENDCASE;
				\
				ENDCASE;
			\
		ENDCASE;
	END 	(*must be error*)
