#lang racket


; calculate damage to deal based on defense and attack
; power. The damage is power minus defense, unless this
; difference is less than 1, in which case make it is
; their quotient
(define attack (lambda (power defense) (
	if (<= 1 (- power defense) )
	(- power defense )
	(/ power defense )
) ) )
	