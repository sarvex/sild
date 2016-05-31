;-------------------------;
; Sild Is a Lisp Dialect! ;
;-------------------------;

(quote (sild is a lisp dialect!))

; That was a quoted list! When the interpreter interprets (quote ...), it just
; returns whatever it has been given, without trying to evaluate it. "quote" is a special form.

'(it also supports the single quote macro! It doesn't
 matter what is in this list since it's preceded with a ' character!)

; Sild doesn't really have types yet...
; but it knows how to distinguish nested lists from symbols!

'(this is a (nested (list) full of) symbols. (or labels, whatever). Arbitrary strings really!)

'these 'are 'symbols 'in 'isolation

; They don't mean anything yet, so they have to be quoted, otherwise sild will
; complain that they don't mean anything when it tries to interpret them. Try
; just typing a word and running this file. Or just uncomment the one below if
; you can't think of anything.

; derp

; The interpreter simply reads a file character by character, parsing as it
; goes, and interpreting each form in turn.

; We have a few special forms for dealing with lists! Here is a list or three labels:

'(a b c)

; We can add something to the head of that list with the keyword "cons"

(cons 'z '(a b c))

; Look how I had to quote both of the arguments, because cons will try to
; evaluate them before putting them together!  I would like for you to be able to
; see that as output! Here is another special form: "display"

(display
    (cons 'z '(a b c))
)

; display is special because it has no return value, so it can only exist on
; the top level. If you try to nest it, you'll get an error. Uncomment the
; following to see that:

; (display (display (cons 'z '(a b c))))

; you can wrap any form in this file with a display call and see the output if
; you want!

; Sild also has 'car' and 'cdr', in classic lisp styleeeee:

; returns the first element in a list
(car '(a b c)) ; a
; returns the list without the first element
(cdr '(a b c)) ; (b c)

; because these applications evaluate their arguments, you can nest them
; arbitrarily!

(cons (car (cdr (cdr '(a b c))))
      (cdr (cdr '(r w q)))) ; outputs (c q)

; next we have cond, which looks like this at its most basic:

'(cond statement if_clause else_clause)

; you can pass any number of pairs of statement/if_clauses in, it will evaluate
; and return the first if_clause that is paired with a truthy statement

'(cond
    statement if_clause
    statement if_clause
    statement if_clause
    statement if_clause
    statement if_clause
    else_clause)

; If no else_clause is provided, and no truthy statements are found, the
; interpreter will return the empty list:

'()

; What is "truthyness", then? Anything that "is" is truthy. The only thing that
; "isn't" is nothing, which is the empty list. Here, this one will actually run:

(cond
  '() 'a
  '() 'b
  'c)

; it returns "c", because the other two statements are false.

(cond
  '() 'a
  'whatever! 'b
  'c)

; this one will return "b", since "whatever!" is truthy!

; there are two "boolean" functions that you can use as statements!
; "eq" tests if two things are the same label, or both the empty list.

(eq 'a 'a) ; true
(eq 'a 'z) ; () which is the empty list which is false!

; and "atom" which will return true for anything that isn't a list with something in it!

(atom 'a) ; true
(atom '()) ; true
(atom '(stuff stuff stuff)) ; ()

; sild has top level definitions for assigning values to labels, too!

(define thingy '(this that theotherthing))

; now thingy can be used in later forms!

(car thingy) ; this

; just like "display", "define" can't be used inside any other form.

; Finally, sild has lambdas! lambda is a special form that produces a procedure
; that can then be applied. It takes two arguments- a list of arguments that it
; accepts and a procedure body. Here is the identity function:

(lambda (x) x)

; That could be applied to anything!

((lambda (x) x) '(1 2 3)) ; (1 2 3)
((lambda (x) x) 'PABST_BLUE_RIBBON) ; PABST_BLUE_RIBBON

; combine "define" with "lambda" and you've got yourself named procedures...

(define cadr
    (lambda (list)
        (car (cdr list))))

(cadr '(1 2 3)) ; 2

; Holy moly! that's the whole language right now. No numbers, no strings, no
; math, no nothing! It's super useless! :D
