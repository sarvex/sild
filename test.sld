(define rev (lambda (x acc)
    (cond
        x (rev (cdr x) (cons (car x) acc))
        acc)))

(display
    (rev '(1 2 3 4 5 6 7 8 9 19) '())
)
