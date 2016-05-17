(define conss (lambda (a d)
               (lambda (m)
                (m a d))))

(define carr (lambda (x)
    (x (lambda (a d) a))))
(define cdrr (lambda (x)
    (x (lambda (a d) d))))

(display
    (conss '(1) '(2))
)
(display
    (carr (conss '(1) '(2)))
)
(display
    (cdrr (conss '(1) '(2)))
)
