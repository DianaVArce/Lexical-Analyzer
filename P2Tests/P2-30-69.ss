; Uses rule 30 and 69
; 30 -> <action> -> LISTOP1_T <stmt> 
; 69 -> <any_other_token> -> LISTP_T 

(define (main)
  (cdr 'list?))

(main)