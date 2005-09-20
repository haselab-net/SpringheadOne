      SUBROUTINE QDSOLN (KT,COST,QUAD,LDQ,B,LDB,JW,X,RC,DUAL,SLK,
     1   TEMP,OBJ)
C
C     RETURN SOLUTION TO QUADRATIC PROGRAMMING PROBLEM FROM FINAL
C     TABLEAU AND COMPUTE OBJECTIVE VALUE IF REQUESTED.
C     CONVERTING THE PROBLEM TO MINIMIZATION WITH ALL .GE. CONSTRAINTS.        
C
      DOUBLE PRECISION  B,TEMP 
      DIMENSION  KT(1),COST(1),QUAD(LDQ,1),B(LDB,1),JW(1),X(1),RC(1),
     1   DUAL(1),SLK(1),TEMP(1)         
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
      COMMON /QPRCBR/ TOLS(2)
      EQUIVALENCE (TOLS(1),TZERO), (TOLS(2),TPIV)
C
      DOUBLE PRECISION DART, BB 
C
C
C     GET X, RC, DUAL, SLK
  100 CONTINUE 
      KART = MO + 1
      DO 110 J = 1, NO
         X(J) = 0.0
         RC(J) = 0.0
  110 CONTINUE
      DO 120 I = 1, MO
         DUAL(I) = 0.0
         SLK(I) = 0.0
  120 CONTINUE
      DART = 0.0D0
      DO 170 I = 1, P
         BB = B(I,P1)
         IF (DABS(BB) .LT. TZERO) BB = 0.0D0
         J = JW(I)
         IF (J .LT. 0) GO TO 150
         IF (J .GT. NO) GO TO 130
         X(J) = BB 
         GO TO 170
  130    CONTINUE
         K = J - NO
         IF (K .EQ. KART) GO TO 140
         DUAL(K) = BB
         GO TO 170
  140    CONTINUE
         DART = BB
         GO TO 170
  150    CONTINUE 
         J = -J
         IF (J .GT. NO) GO TO 160
         RC(J) = BB
         GO TO 170
  160    CONTINUE
         K = J - NO
         IF (K .EQ. KART) GO TO 170
         SLK(K) = BB
  170 CONTINUE
C
C     ADJUST DUALS IF ANY EQUALITIES
      IF (MORE .EQ. 0) GO TO 200
      DO 190 I = 1, MO
         IF (KT(I) .NE. 0) GO TO 190
         IF (MINMAX .EQ. 1) GO TO 180
         DUAL(I) = DUAL(I) - DART
         GO TO 190     
  180    CONTINUE
         DUAL(I) = DART - DUAL(I)
  190 CONTINUE
C     
C     COMPUTE OBJECTIVE VALUE ONLY IF REQUESTED.
  200 CONTINUE
      IF (KOBJ .EQ. 0) GO TO 250
      OBJ = 0.
      DO 220 I = 1, NO
         TEMP(I) = 0.0D0
         DO 210 J = 1, NO
            TEMP(I) = TEMP(I) + X(J)*QUAD(I,J)
  210    CONTINUE
  220 CONTINUE 
      DO 230 J = 1, NO
         OBJ = OBJ + TEMP(J)*X(J)
  230 CONTINUE
      DO 240 J = 1, NO
         OBJ = OBJ + COST(J)*X(J)
  240 CONTINUE
C
C     FINISHED
  250 CONTINUE
      RETURN
      END
