; Cinthya Rosales and Kevin Drake
; uses rules 33 and 84
; 33 <action> -> OR_T <stmt_list>
; 84 <any_other_taken> -> SQUOTE_T <any_other_token>


; Rule 33
( define ( orFunc)
		( or  10 )
)

;( orFunc )

; Rule 84
( define (squote) ' ' =  8 )

( orFunc )
( squote )
