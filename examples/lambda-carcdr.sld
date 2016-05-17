(define conss (lambda (a d)
               (lambda (m)
                (m a d))))

(define carr (lambda (x)
    (x (lambda (a d) a))))
(define cdrr (lambda (x)
    (x (lambda (a d) d))))

(conss '(1) '(2))
|(carr (conss '(1) '(2)))
|(cdrr (conss '(1) '(2)))
