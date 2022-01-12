; Uses rules 28 and 65
; 28. <action> -> COND_T LPAREN_T <stmt_pair_body>
; 65. <any_other_token> -> NOT_T

(define (main a b)
  (cond ((> a b)
           (display "a is greater"))
        ((<= a b)
           (display "a is not greater")))
  'not)
(main 3 2)

