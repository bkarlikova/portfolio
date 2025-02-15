(define     
    (domain TDD-domain)
    (:requirements :strips :typing :fluents)
    (:types market trader product)
    (:predicates 
        (at ?t ?m) 
        (equal?n ?n)
    )
    (:functions
        (product-amount-market ?p - product ?m - market)
        (product-amount-trader ?p - product ?t - trader)
        (product-amount-trader-desired ?p - product ?t - trader)
        (product-prize-market ?p - product ?m - market) 
        (path-prize ?m ?m - market)
        (cost)
    )
    (:action buy
        :parameters (?m - market ?t - trader ?p - product)
        :precondition (and 
	        (at ?t ?m)
	        ( >= (product-amount-market ?p ?m) 1)
	    )
        :effect(and 
           (decrease(product-amount-market ?p ?m) 1) 
           (increase(product-amount-trader ?p ?t) 1) 
           (increase(cost) (product-prize-market ?p ?m))
        )
    )
    (:action move
        :parameters (?t - trader ?m1 ?m2 - market)
        :precondition (and 
            (at ?t ?m1)
        )
        :effect (and 
            (at ?t ?m2) 
            (not (at ?t ?m1)) 
            (increase(cost) (path-prize ?m1 ?m2))
        )
    )
)