(define (sum L)
	(if (null? L)
		0
		(+ (car L) (sum (cdr L)))
	)
)

(define (main)
	(let ([l1 '(3 5)] [l2 '(13 3 4 7 12 5)])
		(display (sum l1))
		(newline)
		(display (sum l2))
		(newline)
		(display (sum (append l1 l2)))
		(newline)
	)
)

(main)
