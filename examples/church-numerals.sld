; ht to http://www.shlomifish.org/lecture/Lambda-Calculus/slides/

(define 0 (lambda (f) (lambda (x) x)))
(define +1 (lambda (n) (lambda (f) (lambda (x) (f ((n f) x))))))

; longhand would be
; (define 1  (lambda (f) (lambda (x) (f x))))
; (define 2  (lambda (f) (lambda (x) (f (f x)))))

(define 1 (+1 0))
(define 2 (+1 1))
(define 3 (+1 2))
(define 4 (+1 3))
(define 5 (+1 4))
(define 6 (+1 5))
(define 7 (+1 6))
(define 8 (+1 7))
(define 9 (+1 8))

(define tolist (lambda (church)
 ((church (lambda (a) (cons 'I a))) '())))

(define +
 (lambda (n)
  (lambda (m)
   ((n +1) m))))

(display
    (tolist ((+ 2) 1))
)
(display
    (tolist ((+ ((+ 4) 4)) 8))
)
