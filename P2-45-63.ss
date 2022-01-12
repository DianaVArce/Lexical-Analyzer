; Skyler Routon
; Uses rules 45 and 63
; 45 <action> -> ROUND_T <stmt>
; 63 <any_other_token> -> AND_T
(define (fortyFiveAndSixtyThree)
	(define total 0)
	(define total2 1.1)
	(round (total2))
	(and (= total 0) (> total2 total))
)

(fortyFiveAndSixtyThree)
; (define (fortyFiveAndSixtyThree)
; 	(define (total) 0)
; 	(define (total2) 1.1)
; 	(round (total2))
; 	(and (= (total) 0) (> (total2) (total)))
; )

; (fortyFiveAndSixtyThree)
; (define (fortyFiveAndSixtyThree)
; 	(round 1.1)
; 	'(and 0 0)
; )
; (fortyFiveAndSixtyThree)
