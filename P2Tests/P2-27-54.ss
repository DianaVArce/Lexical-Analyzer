;#lang racket

; Uses rules 27 and 54
; 27.<action> -> IF_T <stmt> <stmt> <else_part>
; 54.<any_other_token> -> LPAREN_T <more_tokens> RPAREN_T
; test program!!! only testing to see what rules result!!!!

( define (main)
   (if (> 7 5)
       (display "true")
       (display '(1 2 3))
   )
)

(main)



; <stmt> -> <literal>
; <literal> -> SQUOTE_T <quoted_lit>
; <quoted_lit> -> <any_other_token>
; <any_other_token> -> LPAREN_T <more_tokens> RPAREN_T