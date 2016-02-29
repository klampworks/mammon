(define-record-type :rule
  (rule x y)
  rule?
  (x rule-pattern)
  (y rule-score))

(define (score board thread-text)
  (fold-right (lambda (r acc)
               (if (irregex-search (rule-pattern r) thread-text) 
                 (+ acc (rule-score r)) acc))
             0
             rules))
