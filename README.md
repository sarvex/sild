Sild
=====

Sild Is a Lisp Dialect! _Sild_ also means "herring" in Danish. It's also an
anagram of "lisp" if you flip the 'p' around.

```
.        `\\\,
|\ _.--''""````"''--._
  }sild             o `\
|/ '--.._____'`'_._..~~`
`
```

Why did you do this?
------------------

For the learning. I wrote three iterations of this language. The first one was
a disaster! The second I got working pretty well! But then I tried to explain
it to someone and couldn't, so I threw it all out and started over a 3rd time,
and this time I made really atomic commits and wrote blog posts about the whole
thing as I was developing it.

I learned an enormous amount from this project, about Lisp history and
practice, about C, and about language design. It still has a long way to go
before it could be considered useful, but it is nonetheless in a very real
sense a _working programming language_ already.

Install
----------

```
git clone http://github.com/urthbound/sild
cd sild
```

and then...

```
make
```

that should work on OSX! It will probably work on othe systems, too, but you
might have to change the compiler from clang (`cc`) to `gcc` or something!

That will give you a `sild` executable that you can execute or put on your path.

Usage:
------

```
$ sild filename.sld
```

Will run the file specified!

Syntax
------

Sild is a lisp dialect. Right now, it supports the following:

```
quote and '
car
cdr
cons
eq
atom
cond
```
It has two top level only void return value functions!

```
define
display
```

And it has lambdas!

```
lambda
```

For more detailed explanations and examples, see
[examples/basics.sld](http://github.com/urthbound/sild/blob/master/examples/basics.sld)

You can top level bind whatever unicode to these canonical names, I enjoy this
one:

```scheme
(define λ lambda)
```

Todo
----

Oh so, so much! For starters, there's an enormous memory leak with regard to
persistent procedure environments. So I need proper garbage collection!

Also there are no numbers or mathematical operations, or strings or string
operations, or types of any kind really, or system IO.

And it's not properly tail call optimized!

And there is no repl, either! A lisp without a repl, for shame, for shame.

I consider these features to be beyond the working v0.1 prototype presented
here, and look forward to studying how to implement them in the future!
