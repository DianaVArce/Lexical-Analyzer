; Robert Sapien
; Uses rules 48 & 78
; 48 <any_other_token> -> LT_T <stmt_list>
; 78 <any_other_token> -> ROUND_T

(define (main)
  (if (< 1 2)
      (round 4.5)
      'failed
   )
  )

(main)
