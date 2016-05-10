(define rev
    (lambda (x acc)
        (cond
        x (rev (cdr x) (cons (car x) acc))
        acc)))

(display
    (rev '(1 2 3 4 6) '())
)

; (define apply
;     (lambda (f x)
;         (f x)))

; (define consn
;     (lambda (n)
;         (lambda (z)
;             (cons n z))))

; (display
;     ((consn '(1)) '(4))
; )
