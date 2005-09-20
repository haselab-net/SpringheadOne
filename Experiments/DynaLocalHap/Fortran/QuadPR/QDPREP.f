      SUBROUTINE QDPREP (A,LDA,KT,RHS,COST,QUAD,LDQ,B,LDB)
C
C     CONSTRUCT THE INITIAL TABLEAU FOR QUADRATIC PROGRAMMING.
C     IT HAS THE FORM
C             .........................................  
C             .           (T)    .     (T)     .      .
C             . QUAD'+QUAD'      .   -A'       . COST'.
C             .     [1]          .   [2]       . [3]  .      
C     B    =  .........................................
C             .     A'           .    0        . -RHS'.
C             .     [4]          .   [5]       . [6]  .
C             .........................................
C
C     WHERE A',RHS',COST',QUAD' ARE OBTAINED FROM A,RHS,COST,QUAD BY
C     CONVERTING THE PROBLEM TO MINIMIZATION WITH ALL .GE. CONSTRAINTS.        
C
      DOUBLE PRECISION  B 
      DIMENSION  A(LDA,1),KT(1),RHS(1),COST(1),QUAD(LDQ,1),B(LDB,1)  
C
      INTEGER  ROW,COL,INDX,MORE,P,P1,PSQ 
      COMMON /QPRCBI/ INPUTS(14),IOUTS(2),ROW,COL,INDX,MORE,P,P1,PSQ,
     1   NCALL,LENREQ,IO1,IO2,IO3
      EQUIVALENCE (INPUTS(1),ML), (INPUTS(2),NL), (INPUTS(3),MO),
     1   (INPUTS(4),NO), (INPUTS(5),MINMAX), (INPUTS(6),LENWS),
     2   (INPUTS(7),MAXIT), (INPUTS(8),KOBJ), (INPUTS(9),JIT),        
     3   (INPUTS(10),JDATA), (INPUTS(11),JPIVOT), (INPUTS(12),JSOL),
     4   (INPUTS(13),JOUT), (INPUTS(14),JWIDTH),
     5   (IOUTS(1),IERR), (IOUTS(2),ITCNT)
C
C
C     FILL LOWER TRIANGLE OF [1] AND [3].
      N1 = NO - 1
      IF (MINMAX .EQ. 1) GO TO 40
C     MIN. PROBLEM
      B(1,1) = 2.0*QUAD(1,1)
      B(1,P1) = COST(1)
      IF (NO .EQ. 1) GO TO 110
      DO 10 K = 2, NO
         B(K,K) = 2.0*QUAD(K,K)
         B(K,P1) = COST(K)
   10 CONTINUE 
      DO 30 L = 1, N1
         L1 = L + 1
         DO 20 K = L1, NO
            B(K,L) = QUAD(K,L) + QUAD(L,K)
   20    CONTINUE
   30 CONTINUE   
      GO TO 80
C     MAX. PROBLEM
   40 CONTINUE
      B(1,1) = - 2.0*QUAD(1,1)
      B(1,P1) = - COST(1)
      IF (NO .EQ. 1) GO TO 110
      DO 50 K = 2, NO
         B(K,K) = - 2.0*QUAD(K,K)
         B(K,P1) = - COST(K)
   50 CONTINUE 
      DO 70 L = 1, N1
         L1 = L + 1
         DO 60 K = L1, NO
            B(K,L) = - QUAD(K,L) - QUAD(L,K)
   60    CONTINUE
   70 CONTINUE
C
C     FILL UPPER TRAINGLE OF [1].
   80 CONTINUE 
      DO 100 L = 2, NO
         L1 = L - 1
         DO 90 K = 1, L1
            B(K,L) = B(L,K)
   90    CONTINUE
  100 CONTINUE
C
C     CHECK FOR CASE OF NO CONSTRAINTS.  
  110 CONTINUE
      IF (MO .EQ. 0) GO TO 240
C
C     FILL [5].
      N1 = NO + 1
      DO 130 L = N1, P
         DO 120 K = N1, P
            B(K,L) = 0.0
  120    CONTINUE 
  130 CONTINUE
C
C     IF THERE ARE R .EQ. CONSTRAINTS, EXPRESS AS R+1 .GE. CONSTRAINTS,
C     THE LATTER BEING MINUS THE SUM OF THE OTHERS. 
      IF (MORE .EQ. 0) GO TO 150  
      DO 140 L = 1, NO
         B(P,L) = 0.0
  140 CONTINUE
      B(P,P1) = 0.0
C
C     FILL [4] AND [6].
  150 CONTINUE
      DO 210 I = 1, MO
         K = NO + I
         IF (KT(I) .LT. 0) GO TO 170
         IF (KT(I) .GT. 0) GO TO 190
C        .EQ. CONSTRAINT
         DO 160 L = 1, NO
            B(K,L) = A(I,L)
            B(P,L) = B(P,L) - B(K,L)
  160    CONTINUE
         B(K,P1) = - RHS(I)
         B(P,P1) = B(P,P1) - B(K,P1)
         GO TO 210
C        .GE. CONSTRAINT
  170    CONTINUE
         DO 180 L = 1, NO
            B(K,L) = A(I,L)
  180    CONTINUE
         B(K,P1) = - RHS(I)
         GO TO 210
C        .LE. CONSTRAINT
  190    CONTINUE
         DO 200 L = 1, NO
            B(K,L) = - A(I,L)
  200    CONTINUE
         B(K,P1) = RHS(I)
  210 CONTINUE
C
C     FILL [2].
      DO 230 L = 1, NO
         DO 220 K = N1, P
            B(L,K) = - B(K,L)
  220    CONTINUE
  230 CONTINUE
C
C     FINISHED
  240 CONTINUE
      RETURN
      END
