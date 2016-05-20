; ht to http://www.shlomifish.org/lecture/Lambda-Calculus/slides/

(define λ lambda)

(define 0 (λ (f) (λ (x) x)))
(define +1 (λ (n) (λ (f) (λ (x) (f ((n f) x))))))

(define pred_next_tuple
 (λ (tuple)
  (cons (car (cdr tuple)) (cons (+1 (car (cdr tuple))) '()))))
(define -1 (λ (n) (car ((n pred_next_tuple) (cons 0 (cons 0 '()))))))

(define +
 (λ (n)
  (λ (m)
   ((n +1) m))))

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

(define 100
    ((+ ((+ ((+ ((+ ((+ ((+ ((+ ((+ ((+ 10) 10)) 10)) 10)) 10)) 10)) 10)) 10)) 10)) 10))

(define tolist (λ (church)
 ((church (λ (a) (cons 'I a))) '())))
(define null (λ (l) (eq l '())))
(define reverse
 (lambda (l acc)
  (cond l (reverse (cdr l) (cons (car l) acc))
  acc)))

(define cn->numinner (λ (cn nums)
    (cond (tolist cn) (cn->numinner (-1 cn) (cdr nums))
          (car nums))))

(define cn->num
    (λ (cn) (cn->numinner cn '(101 100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1))))

; fizzbuzz

(define fizzbuzzinner
    (λ (cn l three five acc)
        (cond
            (null l) acc
            (null three) (cond (null five) (fizzbuzzinner (-1 cn) (cdr l) (tolist 2) (tolist 4) (cons 'fizzbuzz acc))
                                           (fizzbuzzinner (-1 cn) (cdr l) (tolist 2) (cdr five) (cons 'fizz acc)))
            (null five) (fizzbuzzinner (-1 cn) (cdr l) (cdr three) (tolist 4) (cons 'buzz acc))
                        (fizzbuzzinner (-1 cn) (cdr l) (cdr three) (cdr five) (cons (cn->num cn) acc)))))

(define fizzbuzz (λ ()
    (reverse (fizzbuzzinner 100 (tolist 100) (tolist 2) (tolist 4) '()) '())))

(display (fizzbuzz))
