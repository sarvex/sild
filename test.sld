(define thing (lambda (x)
    (cons (car (cdr x)) (cdr x))))


(define x '(1 2 3))

(display
    (thing x))

