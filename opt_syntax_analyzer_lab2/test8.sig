PROGRAM PARSERTEST8;
    BEGIN
        LOOP (*many rows &$%;")" many rows ()*
comment *)  LOOP
        		CASE 100 5 OF 10000 MOD 10:      (*error*)
        			/
        				CASE 5*2 OF
        				ENDCASE;
        			\
        		ENDCASE;
        	ENDLOOP;
        ENDLOOP;
        CASE X +- Y OF C: /                     (*error*)
        	LOOP
        	ENDLOOP; (**) (*************) (*    *)
        \
        ENDCASE;
    END.