(define consn
    (lambda (n)
        (lambda (z)
            (cons n z))))


(define thing
    (consn '(1)))

(define thing2
    (consn '(2)))

(display
    (thing '(22)))
(display
    (thing2 '(2)))

; ((1) 2) !!!

