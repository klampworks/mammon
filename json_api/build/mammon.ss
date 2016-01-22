(import (scheme r5rs) (srfi 9) (srfi 1))
(load "irregex.scm")

(define-record-type :rule
  (rule x y)
  rule?
  (x rule-pattern)
  (y rule-score))

(define rules
  (list
    (rule (irregex "a" 'i) 10)
    (rule (irregex "p" 'i) 5)))

(define (score thread-text)
  (fold-right (lambda (r acc)
               (if (irregex-search (rule-pattern r) thread-text) 
                 (+ acc (rule-score r)) acc))
             0
             rules))

(display
  (score "apple"))
(newline)

(display
  (score "tomoato"))
(newline)
