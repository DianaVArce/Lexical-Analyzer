
(define (mymember e x)
  (cond ((null? x) f)
        ((equal? e (car x)) t)
        (else (mymember e (cadr x)))))