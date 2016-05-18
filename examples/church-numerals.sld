; ht to http://www.shlomifish.org/lecture/Lambda-Calculus/slides/

(define λ lambda)

(define 0 (λ (f) (λ (x) x)))
(define +1 (λ (n) (λ (f) (λ (x) (f ((n f) x))))))

; longhand would be
; (define 1  (λ (f) (λ (x) (f x))))
; (define 2  (λ (f) (λ (x) (f (f x)))))

(define 1 (+1 0))
(define 2 (+1 1))
(define 3 (+1 2))
(define 4 (+1 3))
(define 5 (+1 4))
(define 6 (+1 5))
(define 7 (+1 6))
(define 8 (+1 7))
(define 9 (+1 8))
(define 10 (+1 9))

(define tolist (λ (church)
 ((church (λ (a) (cons 'I a))) '())))

(define +
 (λ (n)
  (λ (m)
   ((n +1) m))))

(define 100
    ((+ ((+  ((+  ((+  ((+ ((+ ((+ ((+ ((+ 10) 10)) 10)) 10)) 10)) 10)) 10)) 10)) 10)) 10))

(define null (λ (l) (eq l '())))
(define reverse
 (lambda (l acc)
  (cond l (reverse (cdr l) (cons (car l) acc))
  acc)))


; fizzbuzz

(define fizzbuzzinner
    (λ (l three five acc)
        (cond
            (null l) acc
            (null three) (cond (null five) (fizzbuzzinner (cdr l) (tolist 2) (tolist 4) (cons 'fizzbuzz acc))
                                           (fizzbuzzinner (cdr l) (tolist 2) (cdr five) (cons 'fizz acc)))
            (null five) (fizzbuzzinner (cdr l) (cdr three) (tolist 4) (cons 'buzz acc))
                        (fizzbuzzinner (cdr l) (cdr three) (cdr five) (cons '. acc)))))

(define fizzbuzz (λ (num)
    (reverse (fizzbuzzinner (tolist num) (tolist 2) (tolist 4) '()) '())))

(display (fizzbuzz 10))
