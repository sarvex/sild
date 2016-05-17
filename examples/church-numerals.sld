(define 0 (lambda (f x) x))
(define +1 (lambda (cn) (lambda (f x) (f (cn f x)))))

(define 1 (+1 0))
(define 2 (+1 1))
(define 3 (+1 2))
(define 4 (+1 3))

(define tolist (lambda (cn)
    (cn (lambda (x) (cons '. x)) '())))

(display
    (tolist 2)
)
(display
    (tolist 4)
)
