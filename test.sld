(define consn
    (lambda (n)
        (lambda (z)
            (cons n z))))

(define thing
    (consn '(1)))

(display
    (thing '(2)))
