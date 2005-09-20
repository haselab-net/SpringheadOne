      SUBROUTINE QDCOMP (B,TEMP,U,JZ,JW)  
C
C     COMPUTE THE FINAL TABLEAU FOR QUADRATIC PROGRAMMING GIVEN
C     THE INITIAL TABLEAU USING THE METHOD OF PRINCIPAL PIVOTING.
C
      DOUBLE PRECISION  B,TEMP,U
      INTEGER  JZ,JW
      DIMENSION  B(1),TEMP(1),U(1),JZ(1),JW(1)  
C
      INTEGER  ROW,COL,INDX,P,P1,PSQ 
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
      DOUBLE PRECISION  PIVOT
      COMMON /QPRCBD/ PIVOT
C
      DOUBLE PRECISION  Z,BS,BST
      CHARACTER*48  MSG(4)
C
      DATA MSG / 'ALGORITHM ERROR, INDEX VALUE NON-DECREASING.    ',
     2           'NO SOLUTION, INVALID QUADRATIC COSTS MATRIX.    ',
     3           'NO SOLUTION, INFEASIBLE OR UNBOUNDED PROBLEM.   ',
     4           'ERROR, ITERATION LIMIT REACHED.                 ' /
C
C
C*****INITIALIZATIONS
      ITCNT = 0 
      JFLAG = 0
      INDX = P1  
C*****BASIC VARIABLES GET NEGATIVE VALUES.  
C*****NONBASIC VARIABLES(JZ) GET POSITIVE VALUES.
      DO 30 I = 1, P
         JW(I) = -I  
         JZ(I) = I
   30 CONTINUE
   40 ITEMP = 0
C*****CHECK FOR NONDECREASING INDEX (ALLOW UP TO 5 EQUAL INDEXES FOR 
C*****TOLERANCE PURPOSES).  
      IJ = PSQ
      DO 50 I = 1, P
         IJ = IJ + 1
         IF (B(IJ) .LT. (-TZERO)) ITEMP = ITEMP + 1 
   50 CONTINUE  
      IF (ITEMP .NE. INDX) ICT = 0  
      IF (ITEMP .EQ. INDX) ICT = ICT + 1  
      IF (ITEMP .GT. INDX .OR. ICT .EQ. 5) GO TO 400  
C*****IF THE INDEX IS ZERO YOU ARE DONE.
      IF (ITEMP .EQ. 0) GO TO 300  
      INDX = ITEMP
      IJ = PSQ
      DO 60 I = 1, P
         IJ = IJ + 1
         IF (B(IJ) .LT. (-TZERO)) GO TO 70  
   60 CONTINUE  
   70 IJ = P*(I-1) + I 
C*****CHECK FOR NEGATIVE DIAGONAL ELEMENT (NOT ALLOWED IF POS. SEMIDEF.) 
      IF (B(IJ) .LT. (-TPIV)) GO TO 410
      IF (B(IJ) .GT. TPIV) GO TO 100
C*****IF POTENTIAL PIVOT ELEMENT IS NEAR ZERO, USE CODE BELOW
C*****TO DETERMINE THE BLOCKING VARIABLE.  
      II = I  
   80 ROW = 0 
      TMMP = 1000000. 
      IJ = PSQ
      KJ = P*(II-1)  
      DO 90 J = 1, P
         IJ = IJ + 1
         KJ = KJ + 1
         IF (B(IJ) .LT. (-TZERO)) GO TO 90  
C********IF ALL COL. ELEMENTS ARE NONNEGATIVE, THEN THE DRIVING VARIABLE 
C********IS UNBLOCKED AND THERE IS NO SOLUTION.
         IF (B(KJ) .GE. (-TPIV)) GO TO 90 
         Z = -B(IJ) / B(KJ)
         IF (Z .GE. TMMP) GO TO 90
         TMMP = Z
         ROW = J 
   90 CONTINUE  
      IF (ROW. EQ. 0) GO TO 420
      COL = II
      GO TO 150 
C*****COMPUTE Z = -Q(R)/B(R,R), THE INCREASE IN THE DRIVING VARIABLE. 
  100 IJ = PSQ + I  
      KJ = P*(I-1) + I 
      Z = -B(IJ) / B(KJ)
      COL = I 
  110 BS = 0. 
      ROW = 0 
C*****FIND THE NEW BLOCKING VARIABLE, BASED ON WHICH BASIC VARIABLE IS  
C*****MOST AFFECTED BY THE CHANGE IN THE DRIVING VARIABLE.  
      IJ = (COL-1)*P 
      KJ = PSQ
      DO 140 J = 1, P  
         IJ = IJ + 1
         KJ = KJ + 1
         BST = B(IJ)*Z + B(KJ) 
         IF (B(KJ) .LT. (-TZERO) .OR. BST .GE. (-TPIV)) GO TO 140
         IF (DABS(B(KJ)) .LT. TZERO .AND. B(IJ) .LT. (-TZERO)) GO TO 120        
         IF (B(KJ)) 130, 140, 130 
  120    ROW = J 
         GO TO 150 
  130    BST = -BST / B(KJ)
         IF (BST .LE. BS) GO TO 140
         BS = BST
         ROW = J 
  140 CONTINUE  
      IF (ROW .EQ. 0) GO TO 210
C*****NONPRINCIPAL PIVOT ON B(S,R). 
C*****AT THIS POINT THE BLOCKING VARIABLE IS B(NS,I).
  150 III = -JW(ROW)  
      NFLAG = 1
      GO TO 500 
  160 CONTINUE
      IF (ITCNT .GE. MAXIT) GO TO 440 
      ITCNT = ITCNT + 1 
      JFLAG = 0
C*****PUT TOLERANCE ON B(I,P1) AFTER A NONPRINCIPAL PIVOT.  
      IJ = PSQ + I  
      IF (B(IJ) .LT. (-TZERO)) GO TO 170 
      GO TO 40  
C*****THE NEW DRIVING VARIABLE IS THE COMPLEMENT OF THE OLD BLOCKING
C*****VARIABLE  
  170 DO 180 JJ = 1, P 
         IF (JZ(JJ) .EQ. III) GO TO 190
  180 CONTINUE
C*****NO COMPLEMENT VARIABLE (NOT ALLOWED IF POS. SEMIDEF.)  
      GO TO 410 
  190 COL = JJ
C*****THE DISTINGUISHED VARIABLE IS STILL THE I-TH ROW. 
      IJ = P*(COL-1) + I
      IF (B(IJ) .GT. TPIV) GO TO 200
      II = COL
      GO TO 80  
  200 KJ = PSQ + I  
      Z = -B(KJ) / B(IJ)
      GO TO 110 
C*****PRINCIPAL PIVOT ON B(I,I).
  210 LL = ROW
      ROW = I 
      NFLAG = 2
      GO TO 500 
  220 CONTINUE
      IF (JFLAG .EQ. 1) GO TO 290  
      JFLAG = 1
C*****MUST REARRANGE THE ROWS AND COLUMNS FOR COMPLEMENTARITY
C*****EXCHANGE ROWS 
  230 LLL = 0 
      DO 250 I = 1, P  
      K = IABS(JW(I))  
      IF (K .EQ. I) GO TO 250  
      IJ = K  
      KJ = I  
      DO 240 J = 1, P1 
         Z = B(IJ)
         B(IJ) = B(KJ)
         B(KJ) = Z
         IJ = IJ + P  
         KJ = KJ + P  
  240 CONTINUE
      II = JW(I)  
      JW(I) = JW(K)
      JW(K) = II  
      LLL = LLL + 1 
  250 CONTINUE  
      IF (LLL .GE. 2) GO TO 230
C*****EXCHANGE COLUMNS  
  260 LLL = 0 
      DO 280 J = 1, P  
         K = IABS(JZ(J))  
         IF (K .EQ. J) GO TO 280  
         IJ = P*(K-1)
         KJ = P*(J-1)
         DO 270 I = 1, P  
            IJ = IJ + 1
            KJ = KJ + 1
            Z = B(IJ)
            B(IJ) = B(KJ)
            B(KJ) = Z
  270    CONTINUE
         II = JZ(J)  
         JZ(J) = JZ(K)
         JZ(K) = II  
         LLL = LLL + 1 
  280 CONTINUE  
      IF (LLL .GE. 2) GO TO 260
  290 ROW = LL
      ITCNT = ITCNT + 1 
      IF (JPIVOT .EQ. 1) CALL QRPRT5 
      IF (INDX .GT. 1) GO TO 40
C
C*****NORMAL TERMINATION
  300 IERR = 1 
      GO TO 600 
C
C*****ERROR TERMINATION
  400 K = 1
      IERR = 6
      GO TO 460
  410 K = 2
      IERR = 4
      GO TO 460 
  420 K = 3
      IERR = 2
      GO TO 460 
  440 K = 4  
      IERR = 3 
  460 WRITE (IO1,470) MSG(K),ITCNT 
      IF (JOUT .NE. 0) WRITE (IO3,470) MSG(K),ITCNT
  470 FORMAT( / 1X,A48,I6,' PIVOTS PERFORMED.' / )  
      GO TO 600
C
C*****PIVOT CODE.
C*****QUADPP USES A NORMAL JORDAN PIVOT.
C*****HOWEVER, WE CAN SAVE MEMORY BY USING THE ELEMENTARY MATRIX.
C*****STORE PIVOT ELEMENT
  500 CONTINUE
      KC = P*(COL-1)
      KJ = KC + ROW
      PIVOT = B(KJ)
      Z = 1.0D0 / PIVOT 
C*****STORE ELEMENTARY-VECTOR AND U-VECTOR. 
      IJ = ROW  
      ISAVE = I
      DO 510 I = 1, P1 
         U(I) = B(IJ)
         IJ = IJ + P  
  510 CONTINUE
      IJ = KC 
      DO 520 I = 1, P  
      IJ = IJ + 1
         TEMP(I) = -B(IJ) * Z
  520 CONTINUE  
      TEMP(ROW) = -Z - 1.0D0 
C*****COMPUTE ALL COLUMNS EXCEPT PIVOT COLUMN.  
      IJ = 0  
      DO 550 J = 1, P1 
         IF (J .NE. COL) GO TO 530  
         IJ = IJ + P  
         GO TO 550 
  530    DO 540 I = 1, P  
            IJ = IJ + 1
            B(IJ) = B(IJ) + U(J)*TEMP(I)
  540    CONTINUE  
  550 CONTINUE  
C*****COMPUTE PIVOT COLUMN. 
      IJ = KC 
      DO 560 I = 1, P  
         IJ = IJ + 1
         B(IJ) = -TEMP(I)
  560 CONTINUE
C*****COMPUTE NEW PIVOT ELEMENT.
      B(KJ) = Z
C*****KEEP TRACK OF BASIC AND NONBASIC VARIABLES
      K = JW(ROW)
      JW(ROW) = JZ(COL)
      JZ(COL) = K
      IF (JPIVOT .EQ. 1) CALL QRPRT4  
      I = ISAVE
      GO TO (160, 220), NFLAG 
C
C*****FINISHED
  600 RETURN 
      END
