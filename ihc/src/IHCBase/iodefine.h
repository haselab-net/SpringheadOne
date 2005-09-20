/************************************************************************/
/*                                                                      */
/*HEWを使って開発されている場合                                         */
/*                                                                      */
/* 新規にプロジェクトを作成する際、デバイス名を[Other]で選択するか、    */
/* ご使用されるデバイスに仕様が近いデバイスを選択して下さい。           */
/* [Other]を選択した場合は                                              */
/*  ダウンロードしたヘッダファイルをそのままのファイル名でご使用下さい。*/
/* [Other]以外のデバイスを選択した場合は                                */
/*  ダウンロードしたファイルを iodefine.h に変更頂き、                  */
/*  HEWにより自動生成された同名ファイルに上書きしてご使用ください。     */
/*                                                                      */
/*コマンドラインで開発されている場合                                    */
/*                         （日立製コンパイラ以外で開発されている場合） */
/*ダウンロードしたヘッダファイルをそのままご使用下さい。                */
/*                                                                      */
/*                                                                      */
/*      SH7750 Series Include File                         Ver 2.0      */
/*                                                                      */
/************************************************************************/
#ifndef _IO_DEFINE_H_
#define _IO_DEFINE_H_
struct st_mmu {                                         /* struct MMU   */
              unsigned int      PTEH;                   /* PTEH         */
              unsigned int      PTEL;                   /* PTEL         */
              unsigned int      TTB ;                   /* TTB          */
              unsigned int      TEA ;                   /* TEA          */
              unsigned int      MMUCR;                  /* MMUCR        */
              char              wk[32];                 /*              */
              unsigned int      PTEA;                   /* PTEA         */
};                                                      /*              */
#if 0
struct st_cache {                                       /* struct CACHE */
                unsigned int    CCR;                    /* CCR          */
                 char           wk[24];                 /*              */
                 unsigned int   QACR0;                  /* QACR0        */
                 unsigned int   QACR1;                  /* QACR1        */
};                                                      /*              */
#endif
struct st_cache {                                       /* struct CACHE */
             union {                                    /*  STBCR       */
                   unsigned int       LONG;             /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int    :16;          /*              */
                          unsigned int IIX:1;           /*    IIX       */
                          unsigned int    :3;           /*              */
                          unsigned int ICI:1;           /*    ICI       */
                          unsigned int    :2;           /*              */
                          unsigned int ICE:1;           /*    ICE       */
                          unsigned int OIX:1;           /*    OIX       */
                          unsigned int    :1;           /*              */
                          unsigned int ORA:1;           /*    ORA       */
                          unsigned int    :1;           /*              */
                          unsigned int OCI:1;           /*    OCI       */
                          unsigned int CB :1;           /*    CB        */
                          unsigned int WT :1;           /*    WT        */
                          unsigned int OCE:1;           /*    OCE       */
                          }          BIT;               /*              */
                   }           CCR  ;                   /*              */
              char             wk[24];                  /*              */
              unsigned int     QACR0 ;                  /* QACR0        */
              unsigned int     QACR1 ;                  /* QACR1        */
};                                                      /*              */
struct st_exp {                                         /* struct EXP   */
               unsigned int     TRA;                    /* TRA          */
               unsigned int     EXPEVT;                 /* EXPEVT       */
               unsigned int     INTEVT;                 /* INTEVT       */
};                                                      /*              */
struct st_ubc {                                         /* struct UBC   */
              unsigned char     BASRA;                  /* BASRA        */
              char              wk1[3];                 /*              */
              unsigned char     BASRB;                  /* BASRB Address*/
              char              wk2[2097127];           /*              */
              void             *BARA;                   /* BARA         */
              unsigned char     BAMRA;                  /* BAMRA        */
              char              wk3[3];                 /*              */
              unsigned short    BBRA;                   /* BBRA         */
              char              wk4[2];                 /*              */
              void             *BARB;                   /* BARB         */
              unsigned char     BAMRB;                  /* BAMRB        */
              char              wk5[3];                 /*              */
              unsigned short    BBRB;                   /* BBRB         */
              char              wk6[2];                 /*              */
              unsigned int      BDRB;                   /* BDRB         */
              unsigned int      BDMRB;                  /* BDMRB        */
              union {                                   /* BRCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short CMFA:1;       /*    CMFA      */
                           unsigned short CMFB:1;       /*    CMFB      */
                           unsigned short     :3;       /*              */
                           unsigned short PCBA:1;       /*    PCBA      */
                           unsigned short     :2;       /*              */
                           unsigned short DBEB:1;       /*    DBEB      */
                           unsigned short PCBB:1;       /*    PCBB      */
                           unsigned short     :2;       /*              */
                           unsigned short SEQ :1;       /*    SEQ       */
                           }       BIT;                 /*              */
                    }           BRCR;                   /*              */
};                                                      /*              */
struct st_bsc {                                         /* struct BSC   */
              union {                                   /* BCR1         */
                    unsigned int LONG ;                 /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int ENDIAN:1;       /*    ENDIAN    */
                           unsigned int MASTER:1;       /*    MASTER    */
                           unsigned int A0MPX :1;       /*    A0MPX     */
                           unsigned int       :3;       /*              */
                           unsigned int IPUP  :1;       /*    IPUP      */
                           unsigned int OPUP  :1;       /*    OPUP      */
                           unsigned int       :2;       /*              */
                           unsigned int A1MBC :1;       /*    A1MBC     */
                           unsigned int A4MBC :1;       /*    A4MBC     */
                           unsigned int BREQEN:1;       /*    BREQEN    */
                           unsigned int PSHR  :1;       /*    PSHR      */
                           unsigned int MEMMPX:1;       /*    MEMMPX    */
                           unsigned int       :1;       /*              */
                           unsigned int HIZMEM:1;       /*    HIZMEM    */
                           unsigned int HIZCNT:1;       /*    HIZCNT    */
                           unsigned int A0BST :3;       /*    A0BST     */
                           unsigned int A5BST :3;       /*    A5BST     */
                           unsigned int A6BST :3;       /*    A6BST     */
                           unsigned int DRAMTP:3;       /*    DRAMTP    */
                           unsigned int       :1;       /*              */
                           unsigned int A56PCM:1;       /*    A56PCM    */
                           }     BIT;                   /*              */
                    }           BCR1;                   /*              */
              union {                                   /* BCR2         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short A0SZ  :2;     /*    A0SZ      */
                           unsigned short A6SZ  :2;     /*    A6SZ      */
                           unsigned short A5SZ  :2;     /*    A5SZ      */
                           unsigned short A4SZ  :2;     /*    A4SZ      */
                           unsigned short A3SZ  :2;     /*    A3SZ      */
                           unsigned short A2SZ  :2;     /*    A2SZ      */
                           unsigned short A1SZ  :2;     /*    A1SZ      */
                           unsigned short       :1;     /*              */
                           unsigned short PORTEN:1;     /*    PORTEN    */
                           }       BIT;                 /*              */
                    }           BCR2;                   /*              */
              char              wk1[2];                 /*              */
              union {                                   /* WCR1         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int      :1;        /*              */
                           unsigned int DMAIW:3;        /*    DMAIW     */
                           unsigned int      :1;        /*              */
                           unsigned int A6IW :3;        /*    A6IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A5IW :3;        /*    A5IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A4IW :3;        /*    A4IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A3IW :3;        /*    A3IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A2IW :3;        /*    A2IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A1IW :3;        /*    A1IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A0IW :3;        /*    A0IW      */
                           }     BIT;                   /*              */
                    }           WCR1;                   /*              */
              union {                                   /* WCR2         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int A6W:3;          /*    A6W       */
                           unsigned int A6B:3;          /*    A6B       */
                           unsigned int A5W:3;          /*    A5W       */
                           unsigned int A5B:3;          /*    A5B       */
                           unsigned int A4W:3;          /*    A4W       */
                           unsigned int    :1;          /*              */
                           unsigned int A3W:3;          /*    A3W       */
                           unsigned int    :1;          /*              */
                           unsigned int A2W:3;          /*    A2W       */
                           unsigned int A1W:3;          /*    A1W       */
                           unsigned int A0W:3;          /*    A0W       */
                           unsigned int A0B:3;          /*    A0B       */
                           }     BIT;                   /*              */
                    }           WCR2;                   /*              */
              union {                                   /* WCR3         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :5;         /*              */
                           unsigned int A6S0:1;         /*    A6S0      */
                           unsigned int A6H :2;         /*    A6H       */
                           unsigned int     :1;         /*              */
                           unsigned int A5S0:1;         /*    A5S0      */
                           unsigned int A5H :2;         /*    A5H       */
                           unsigned int     :1;         /*              */
                           unsigned int A4S0:1;         /*    A4S0      */
                           unsigned int A4H :2;         /*    A4H       */
                           unsigned int     :1;         /*              */
                           unsigned int A3S0:1;         /*    A3S0      */
                           unsigned int A3H :2;         /*    A3H       */
                           unsigned int     :1;         /*              */
                           unsigned int A2S0:1;         /*    A2S0      */
                           unsigned int A2H :2;         /*    A2H       */
                           unsigned int     :1;         /*              */
                           unsigned int A1S0:1;         /*    A1S0      */
                           unsigned int A1H :2;         /*    A1H       */
                           unsigned int     :1;         /*              */
                           unsigned int A0S0:1;         /*    A0S0      */
                           unsigned int A0H :2;         /*    A0H       */
                           }     BIT;                   /*              */
                    }           WCR3;                   /*              */
              union {                                   /* MCR          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int RASD   :1;      /*    RASD      */
                           unsigned int MRSET  :1;      /*    MRSET     */
                           unsigned int TRC    :3;      /*    TRC       */
                           unsigned int        :3;      /*              */
                           unsigned int TCAS   :1;      /*    TCAS      */
                           unsigned int        :1;      /*              */
                           unsigned int TPC    :3;      /*    TPC       */
                           unsigned int        :1;      /*              */
                           unsigned int RCD    :2;      /*    RCD       */
                           unsigned int TRWL   :3;      /*    TRWL      */
                           unsigned int TRAS   :3;      /*    TRAS      */
                           unsigned int BE     :1;      /*    BE        */
                           unsigned int SZ     :2;      /*    SZ        */
                           unsigned int AMXEXT :1;      /*    AMXEXT    */
                           unsigned int AMX    :3;      /*    AMX       */
                           unsigned int RFSH   :1;      /*    RFSH      */
                           unsigned int RMODE  :1;      /*    RMODE     */
                           unsigned int EDOMODE:1;      /*    EDOMODE   */
                           }     BIT;                   /*              */
                    }           MCR;                    /*              */
              union {                                   /* PCR          */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short A5PCW:2;      /*    A5PCW     */
                           unsigned short A6PCW:2;      /*    A6PCW     */
                           unsigned short A5TED:3;      /*    A5TED     */
                           unsigned short A6TED:3;      /*    A6TED     */
                           unsigned short A5TEH:3;      /*    A5TEH     */
                           unsigned short A6TEH:3;      /*    A6TEH     */
                           }       BIT;                 /*              */
                    }           PCR;                    /*              */
              char              wk2[2];                 /*              */
              union {                                   /* RTCSR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short CMF :1;       /*    CMF       */
                           unsigned short CMIE:1;       /*    CMIE      */
                           unsigned short CKS :3;       /*    CKS       */
                           unsigned short OVF :1;       /*    OVF       */
                           unsigned short OVIE:1;       /*    OVIE      */
                           unsigned short LMTS:1;       /*    LMTS      */
                           }       BIT;                 /*              */
                    }           RTCSR;                  /*              */
              char              wk3[2];                 /*              */
              unsigned short    RTCNT;                  /* RTCNT        */
              char              wk4[2];                 /*              */
              unsigned short    RTCOR;                  /* RTCOR        */
              char              wk5[2];                 /*              */
              unsigned short    RFCR;                   /* RFCR         */
};                                                      /*              */
struct st_port {                                        /* struct PORT  */
               union {                                  /* PCTRA        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int PB15PUP:1;     /*    PB15PUP   */
                            unsigned int PB15IO :1;     /*    PB15IO    */
                            unsigned int PB14PUP:1;     /*    PB14PUP   */
                            unsigned int PB14IO :1;     /*    PB14IO    */
                            unsigned int PB13PUP:1;     /*    PB13PUP   */
                            unsigned int PB13IO :1;     /*    PB13IO    */
                            unsigned int PB12PUP:1;     /*    PB12PUP   */
                            unsigned int PB12IO :1;     /*    PB12IO    */
                            unsigned int PB11PUP:1;     /*    PB11PUP   */
                            unsigned int PB11IO :1;     /*    PB11IO    */
                            unsigned int PB10PUP:1;     /*    PB10PUP   */
                            unsigned int PB10IO :1;     /*    PB10IO    */
                            unsigned int PB9PUP :1;     /*    PB9PUP    */
                            unsigned int PB9IO  :1;     /*    PB9IO     */
                            unsigned int PB8PUP :1;     /*    PB8PUP    */
                            unsigned int PB8IO  :1;     /*    PB8IO     */
                            unsigned int PB7PUP :1;     /*    PB7PUP    */
                            unsigned int PB7IO  :1;     /*    PB7IO     */
                            unsigned int PB6PUP :1;     /*    PB6PUP    */
                            unsigned int PB6IO  :1;     /*    PB6IO     */
                            unsigned int PB5PUP :1;     /*    PB5PUP    */
                            unsigned int PB5IO  :1;     /*    PB5IO     */
                            unsigned int PB4PUP :1;     /*    PB4PUP    */
                            unsigned int PB4IO  :1;     /*    PB4IO     */
                            unsigned int PB3PUP :1;     /*    PB3PUP    */
                            unsigned int PB3IO  :1;     /*    PB3IO     */
                            unsigned int PB2PUP :1;     /*    PB2PUP    */
                            unsigned int PB2IO  :1;     /*    PB2IO     */
                            unsigned int PB1PUP :1;     /*    PB1PUP    */
                            unsigned int PB1IO  :1;     /*    PB1IO     */
                            unsigned int PB0PUP :1;     /*    PB0PUP    */
                            unsigned int PB0IO  :1;     /*    PB0IO     */
                            }     BIT;                  /*              */
                     }          PCTRA;                  /*              */
               union {                                  /* PDTRA        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short PB15DT:1;    /*    PB15DT    */
                            unsigned short PB14DT:1;    /*    PB14DT    */
                            unsigned short PB13DT:1;    /*    PB13DT    */
                            unsigned short PB12DT:1;    /*    PB12DT    */
                            unsigned short PB11DT:1;    /*    PB11DT    */
                            unsigned short PB10DT:1;    /*    PB10DT    */
                            unsigned short PB9DT :1;    /*    PB9DT     */
                            unsigned short PB8DT :1;    /*    PB8DT     */
                            unsigned short PB7DT :1;    /*    PB7DT     */
                            unsigned short PB6DT :1;    /*    PB6DT     */
                            unsigned short PB5DT :1;    /*    PB5DT     */
                            unsigned short PB4DT :1;    /*    PB4DT     */
                            unsigned short PB3DT :1;    /*    PB3DT     */
                            unsigned short PB2DT :1;    /*    PB2DT     */
                            unsigned short PB1DT :1;    /*    PB1DT     */
                            unsigned short PB0DT :1;    /*    PB0DT     */
                            }       BIT;                /*              */
                     }           PDTRA;                 /*              */
               char              wk1[14];               /*              */
               union {                                  /* PCTRB        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int        :24;    /*              */
                            unsigned int PB19PUP: 1;    /*    PB19PUP   */
                            unsigned int PB19IO : 1;    /*    PB19IO    */
                            unsigned int PB18PUP: 1;    /*    PB18PUP   */
                            unsigned int PB18IO : 1;    /*    PB18IO    */
                            unsigned int PB17PUP: 1;    /*    PB17PUP   */
                            unsigned int PB17IO : 1;    /*    PB17IO    */
                            unsigned int PB16PUP: 1;    /*    PB16PUP   */
                            unsigned int PB16IO : 1;    /*    PB16IO    */
                            }     BIT;                  /*              */
                     }          PCTRB;                  /*              */
               union {                                  /* PDTRB        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short       :12;   /*              */
                            unsigned short PB19DT: 1;   /*    PB19DT    */
                            unsigned short PB18DT: 1;   /*    PB18DT    */
                            unsigned short PB17DT: 1;   /*    PB17DT    */
                            unsigned short PB16DT: 1;   /*    PB16DT    */
                            }       BIT;                /*              */
                     }          PDTRB;                  /*              */
               char             wk2[2];                 /*              */
               union {                                  /* GPIOIC       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short PTIREN15:1;  /*    PTIREN15  */
                            unsigned short PTIREN14:1;  /*    PTIREN14  */
                            unsigned short PTIREN13:1;  /*    PTIREN13  */
                            unsigned short PTIREN12:1;  /*    PTIREN12  */
                            unsigned short PTIREN11:1;  /*    PTIREN11  */
                            unsigned short PTIREN10:1;  /*    PTIREN10  */
                            unsigned short PTIREN9 :1;  /*    PTIREN9   */
                            unsigned short PTIREN8 :1;  /*    PTIREN8   */
                            unsigned short PTIREN7 :1;  /*    PTIREN7   */
                            unsigned short PTIREN6 :1;  /*    PTIREN6   */
                            unsigned short PTIREN5 :1;  /*    PTIREN5   */
                            unsigned short PTIREN4 :1;  /*    PTIREN4   */
                            unsigned short PTIREN3 :1;  /*    PTIREN3   */
                            unsigned short PTIREN2 :1;  /*    PTIREN2   */
                            unsigned short PTIREN1 :1;  /*    PTIREN1   */
                            unsigned short PTIREN0 :1;  /*    PTIREN0   */
                            }     BIT;                  /*              */
                     }          GPIOIC;                 /*              */
};                                                      /*              */
struct st_dmac {                                        /* struct DMAC  */
               union {                                  /* DMAOR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :16;       /*              */
                            unsigned int DDT : 1;       /*    DDT       */
                            unsigned int     : 5;       /*              */
                            unsigned int PR  : 2;       /*    PR        */
                            unsigned int     : 3;       /*              */
                            unsigned int COD : 1;       /*    COD       */
                            unsigned int     : 1;       /*              */
                            unsigned int AE  : 1;       /*    AE        */
                            unsigned int NMIF: 1;       /*    NMIF      */
                            unsigned int DME : 1;       /*    DME       */
                            }     BIT;                  /*              */
                     }          DMAOR;                  /*              */
};                                                      /*              */
struct st_dmac0 {                                       /* struct DMAC0 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int SSA:3;        /*    SSA       */
                             unsigned int STC:1;        /*    STC       */
                             unsigned int DSA:3;        /*    DSA       */
                             unsigned int DTC:1;        /*    DTC       */
                             unsigned int    :4;        /*              */
                             unsigned int DS :1;        /*    DS        */
                             unsigned int RL :1;        /*    RL        */
                             unsigned int AM :1;        /*    AM        */
                             unsigned int AL :1;        /*    AL        */
                             unsigned int DM :2;        /*    DM        */
                             unsigned int SM :2;        /*    SM        */
                             unsigned int RS :4;        /*    RS        */
                             unsigned int TM :1;        /*    TM        */
                             unsigned int TS :3;        /*    TS        */
                             unsigned int    :1;        /*              */
                             unsigned int IE :1;        /*    IE        */
                             unsigned int TE :1;        /*    TE        */
                             unsigned int DE :1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_dmac2 {                                       /* struct DMAC2 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int SSA:3;        /*    SSA       */
                             unsigned int STC:1;        /*    STC       */
                             unsigned int DSA:3;        /*    DSA       */
                             unsigned int DTC:1;        /*    DTC       */
                             unsigned int    :4;        /*              */
                             unsigned int DS :1;        /*    DS        */
                             unsigned int    :1;        /*              */
                             unsigned int AM :1;        /*    AM        */
                             unsigned int    :1;        /*              */
                             unsigned int DM :2;        /*    DM        */
                             unsigned int SM :2;        /*    SM        */
                             unsigned int RS :4;        /*    RS        */
                             unsigned int TM :1;        /*    TM        */
                             unsigned int TS :3;        /*    TS        */
                             unsigned int    :1;        /*              */
                             unsigned int IE :1;        /*    IE        */
                             unsigned int TE :1;        /*    TE        */
                             unsigned int DE :1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_cpg {                                         /* struct CPG   */
              union {                                   /* FRQCR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short       :4;     /*              */
                           unsigned short CKOEN :1;     /*    CKOEN     */
                           unsigned short PLL1EN:1;     /*    PLL1EN    */
                           unsigned short PLL2EN:1;     /*    PLL2EN    */
                           unsigned short IFC   :3;     /*    IFC       */
                           unsigned short BFC   :3;     /*    BFC       */
                           unsigned short PFC   :3;     /*    PFC       */
                           }       BIT;                 /*              */
                    }           FRQCR;                  /*              */
              char              wk1[6];                 /*              */
              union {                                   /* WTCNT        */
                    unsigned char  READ;                /*  Read  Access*/
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCNT;                  /*              */
              char              wk2[2];                 /*              */
              union {                                   /* WTCSR        */
                    union {                             /*  Read  Access*/
                          unsigned char BYTE;           /*   Byte Access*/
                          struct {                      /*   Bit  Access*/
                                 unsigned char TME :1;  /*    TME       */
                                 unsigned char WTIT:1;  /*    WT/IT     */
                                 unsigned char RSTS:1;  /*    RSTS      */
                                 unsigned char WOVF:1;  /*    WOVF      */
                                 unsigned char IOVF:1;  /*    IOVF      */
                                 unsigned char CKS :3;  /*    CKS       */
                                 }      BIT;            /*              */
                         }         READ;                /*              */
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCSR;                  /*              */
};                                                      /*              */
struct st_stb {                                         /* struct STB   */
              union {                                   /* STBCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char STBY :1;       /*    STBY      */
                           unsigned char PHZ  :1;       /*    PHZ       */
                           unsigned char PPU  :1;       /*    PPU       */
                           unsigned char _DMAC:1;       /*    DMAC      */
                           unsigned char _SCIF:1;       /*    SCIF      */
                           unsigned char _TMU :1;       /*    TMU       */
                           unsigned char _RTC :1;       /*    RTC       */
                           unsigned char _SCI :1;       /*    SCI       */
                           }      BIT;                  /*              */
                    }           CR;                     /*              */
              char              wk[11];                 /*              */
              union {                                   /* STBCR2       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char DSLP:1;        /*    DSLP      */
                           unsigned char     :5;        /*              */
                           unsigned char _SQ :1;        /*    SQ        */
                           unsigned char _UBC:1;        /*    UBC       */
                           }      BIT;                  /*              */
                    }           CR2;                    /*              */
};                                                      /*              */
struct st_rtc {                                         /* struct RTC   */
              unsigned char     R64CNT;                 /* R64CNT       */
              char              wk1[3];                 /*              */
              union {                                   /* RSECCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :1;         /*              */
                           unsigned char S10:3;         /*     10sec    */
                           unsigned char S1 :4;         /*      1sec    */
                           }      BIT;                  /*              */
                    }           RSECCNT;                /*              */
              char              wk2[3];                 /*              */
              union {                                   /* RMINCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :1;         /*              */
                           unsigned char M10:3;         /*     10min    */
                           unsigned char M1 :4;         /*      1min    */
                           }      BIT;                  /*              */
                    }           RMINCNT;                /*              */
              char              wk3[3];                 /*              */
              union {                                   /* RHRCNT       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :2;         /*              */
                           unsigned char H10:2;         /*     10sec    */
                           unsigned char H1 :4;         /*      1sec    */
                           }      BIT;                  /*              */
                    }           RHRCNT;                 /*              */
              char              wk4[3];                 /*              */
              union {                                   /* RWKCNT       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :5;          /*              */
                           unsigned char WK:3;          /*     week     */
                           }      BIT;                  /*              */
                    }           RWKCNT;                 /*              */
              char              wk5[3];                 /*              */
              union {                                   /* RDAYCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :2;         /*              */
                           unsigned char D10:2;         /*     10day    */
                           unsigned char D1 :4;         /*      1day    */
                           }      BIT;                  /*              */
                    }           RDAYCNT;                /*              */
              char              wk6[3];                 /*              */
              union {                                   /* RMONCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :3;         /*              */
                           unsigned char M10:1;         /*     10mon    */
                           unsigned char M1 :4;         /*      1mon    */
                           }      BIT;                  /*              */
                    }           RMONCNT;                /*              */
              char              wk7[3];                 /*              */
              union {                                   /* RYRCNT       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short Y1000:4;      /*   1000year   */
                           unsigned short Y100 :4;      /*    100year   */
                           unsigned short Y10  :4;      /*     10year   */
                           unsigned short Y1   :4;      /*      1year   */
                           }       BIT;                 /*              */
                    }           RYRCNT;                 /*              */
              char              wk8[2];                 /*              */
              union {                                   /* RSECAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char S10:3;         /*     10sec    */
                           unsigned char S1 :4;         /*      1sec    */
                           }      BIT;                  /*              */
                    }           RSECAR;                 /*              */
              char              wk9[3];                 /*              */
              union {                                   /* RMINAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char M10:3;         /*     10min    */
                           unsigned char M1 :4;         /*      1min    */
                           }      BIT;                  /*              */
                    }           RMINAR;                 /*              */
              char              wk10[3];                /*              */
              union {                                   /* RHRAR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char    :1;         /*              */
                           unsigned char H10:2;         /*     10sec    */
                           unsigned char H1 :4;         /*      1sec    */
                           }      BIT;                  /*              */
                    }           RHRAR;                  /*              */
              char              wk11[3];                /*              */
              union {                                   /* RWKAR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char    :4;         /*              */
                           unsigned char WK :3;         /*     week     */
                           }      BIT;                  /*              */
                    }           RWKAR;                  /*              */
              char              wk12[3];                /*              */
              union {                                   /* RDAYAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char    :1;         /*              */
                           unsigned char D10:2;         /*     10day    */
                           unsigned char D1 :4;         /*      1day    */
                           }      BIT;                  /*              */
                    }           RDAYAR;                 /*              */
              char              wk13[3];                /*              */
              union {                                   /* RMONAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*              */
                           unsigned char    :2;         /*              */
                           unsigned char M10:1;         /*     10mon    */
                           unsigned char M1 :4;         /*      1mon    */
                           }      BIT;                  /*              */
                    }           RMONAR;                 /*              */
              char              wk14[3];                /*              */
              union {                                   /* RCR1         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CF :1;         /*     CF       */
                           unsigned char    :2;         /*              */
                           unsigned char CIE:1;         /*     CIE      */
                           unsigned char AIE:1;         /*     AIE      */
                           unsigned char    :2;         /*              */
                           unsigned char AF :1;         /*     AF       */
                           }      BIT;                  /*              */
                    }           RCR1;                   /*              */
              char              wk15[3];                /*              */
              union {                                   /* RCR2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PEF  :1;       /*     PEF      */
                           unsigned char PES  :3;       /*     PES      */
                           unsigned char RTCEN:1;       /*     RTCEN    */
                           unsigned char ADJ  :1;       /*     ADJ      */
                           unsigned char RESET:1;       /*     RESET    */
                           unsigned char START:1;       /*     START    */
                           }      BIT;                  /*              */
                    }           RCR2;                   /*              */
};                                                      /*              */
struct st_tmu  {                                        /* struct TMU   */
               union {                                  /* TOCR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :7;       /*              */
                            unsigned char TCOE:1;       /*    TCOE      */
                            }       BIT;                /*              */
                     }          TOCR;                   /*              */
               char             wk[3];                  /*              */
               union {                                  /* TSTR         */
                     unsigned char  BYTE;               /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :5;       /*              */
                            unsigned char STR2:1;       /*    STR2      */
                            unsigned char STR1:1;       /*    STR1      */
                            unsigned char STR0:1;       /*    STR0      */
                            }       BIT;                /*              */
                     }          TSTR;                   /*              */
};                                                      /*              */
struct st_tmu0 {                                        /* struct TMU0  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :7;      /*              */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short     :2;      /*              */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
};                                                      /*              */
struct st_tmu2 {                                        /* struct TMU2  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :6;      /*              */
                            unsigned short ICPF:1;      /*    ICPF      */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short ICPE:2;      /*    ICPE      */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
               unsigned int     TCPR;                   /* TCPR         */
};                                                      /*              */
struct st_intc {                                        /* struct INTC  */
               union {                                  /* ICR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short NMIL:1;      /*    NMIL      */
                            unsigned short MAI :1;      /*    MAI       */
                            unsigned short     :4;      /*              */
                            unsigned short NMIB:1;      /*    NMIB      */
                            unsigned short NMIE:1;      /*    NMIE      */
                            unsigned short IRLM:1;      /*    IRLM      */
                            }       BIT;                /*              */
                     }          ICR;                    /*              */
               char             wk1[2];                 /*              */
               union {                                  /* IPRA         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _TMU0:4;     /*    TMU0      */
                            unsigned short _TMU1:4;     /*    TMU1      */
                            unsigned short _TMU2:4;     /*    TMU2      */
                            unsigned short _RTC :4;     /*    RTC       */
                            }       BIT;                /*              */
                     }          IPRA;                   /*              */
               char             wk2[2];                 /*              */
               union {                                  /* IPRB         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _WDT:4;      /*    WDT       */
                            unsigned short _REF:4;      /*    REF       */
                            unsigned short _SCI:4;      /*    SCI       */
                            }       BIT;                /*              */
                     }          IPRB;                   /*              */
               char             wk3[2];                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _GPIO:4;     /*    GPIO      */
                            unsigned short _DMAC:4;     /*    DMAC      */
                            unsigned short _SCIF:4;     /*    SCIF      */
                            unsigned short _HUDI:4;     /*    HUDI      */
                            }       BIT;                /*              */
                     }          IPRC;                   /*              */
               char             wk4[2];                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _IRL0:4;     /*    IRL0      */
                            unsigned short _IRL1:4;     /*    IRL1      */
                            unsigned short _IRL2:4;     /*    IRL2      */
                            unsigned short _IRL3:4;     /*    IRL3      */
                            }       BIT;                /*              */
                     }          IPRD;                   /*              */
};                                                      /*              */
struct st_sci {                                         /* struct SCI   */
              union {                                   /* SCSMR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CA  :1;        /*    C/A       */
                           unsigned char CHR :1;        /*    CHR       */
                           unsigned char PE  :1;        /*    PE        */
                           unsigned char OE  :1;        /*    O/E       */
                           unsigned char STOP:1;        /*    STOP      */
                           unsigned char MP  :1;        /*    MP        */
                           unsigned char CKS :2;        /*    CKS       */
                           }      BIT;                  /*              */
                    }           SCSMR;                  /*              */
              char              wk1[3];                 /*              */
              unsigned char     SCBRR;                  /* SCBRR        */
              char              wk2[3];                 /*              */
              union {                                   /* SCSCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TIE :1;        /*    TIE       */
                           unsigned char RIE :1;        /*    RIE       */
                           unsigned char TE  :1;        /*    TE        */
                           unsigned char RE  :1;        /*    RE        */
                           unsigned char MPIE:1;        /*    MPIE      */
                           unsigned char TEIE:1;        /*    TEIE      */
                           unsigned char CKE :2;        /*    CKE       */
                           }      BIT;                  /*              */
                    }           SCSCR;                  /*              */
              char              wk3[3];                 /*              */
              unsigned char     SCTDR;                  /* SCTDR        */
              char              wk4[3];                 /*              */
              union {                                   /* SCSSR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TDRE:1;        /*    TDRE      */
                           unsigned char RDRF:1;        /*    RDRF      */
                           unsigned char ORER:1;        /*    ORER      */
                           unsigned char FER :1;        /*    FER       */
                           unsigned char PER :1;        /*    PER       */
                           unsigned char TEND:1;        /*    TEND      */
                           unsigned char MPB :1;        /*    MPB       */
                           unsigned char MPBT:1;        /*    MPBT      */
                           }      BIT;                  /*              */
                    }           SCSSR;                  /*              */
              char              wk5[3];                 /*              */
              unsigned char     SCRDR;                  /* SCRDR        */
              char              wk6[3];                 /*              */
              union {                                   /* SCSCMR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :4;        /*              */
                           unsigned char SDIR:1;        /*    SDIR      */
                           unsigned char SINV:1;        /*    SINV      */
                           unsigned char     :1;        /*              */
                           unsigned char SMIF:1;        /*    SMIF      */
                           }      BIT;                  /*              */
                    }           SCSCMR;                 /*              */
              char              wk7[3];                 /*              */
              union {                                   /* SCSPTR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char EIO   :1;      /*    EI0       */
                           unsigned char       :3;      /*              */
                           unsigned char SPB1IO:1;      /*    SPB1IO    */
                           unsigned char SPB1DT:1;      /*    SPB1DT    */
                           unsigned char SPB0IO:1;      /*    SPB0IO    */
                           unsigned char SPB0DT:1;      /*    SPB0DT    */
                           }      BIT;                  /*              */
                    }           SCSPTR;                 /*              */
};                                                      /*              */
struct st_smci {                                        /* struct SMCI  */
               union {                                  /* SCSMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char GM  :1;       /*    GM        */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char PE  :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char MP  :1;       /*    MP        */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SCSMR;                  /*              */
               char             wk1[3];                 /*              */
               unsigned char    SCBRR;                  /* SCBRR        */
               char             wk2[3];                 /*              */
               union {                                  /* SCSCR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char     :2;       /*              */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3[3];                 /*              */
               unsigned char    SCTDR;                  /* SCTDR        */
               char             wk4[3];                 /*              */
               union {                                  /* SCSSR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char ERS :1;       /*    ERS       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            }      BIT;                 /*              */
                     }          SCSSR;                  /*              */
               char             wk5[3];                 /*              */
               unsigned char    SCRDR;                  /* SCRDR        */
               char             wk6[3];                 /*              */
               union {                                  /* SCSCMR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCSCMR;                 /*              */
               char             wk7[3];                 /*              */
               union {                                  /* SCSPTR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char EIO   :1;     /*    EI0       */
                            unsigned char       :3;     /*              */
                            unsigned char SPB1IO:1;     /*    SPB1IO    */
                            unsigned char SPB1DT:1;     /*    SPB1DT    */
                            unsigned char SPB0IO:1;     /*    SPB0IO    */
                            unsigned char SPB0DT:1;     /*    SPB0DT    */
                            }      BIT;                 /*              */
                     }          SCSPTR;                 /*              */
};                                                      /*              */
struct st_scif {                                        /* struct SCIF  */
               union {                                  /* SCSMR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :9;      /*              */
                            unsigned short CHR :1;      /*    CHR       */
                            unsigned short PE  :1;      /*    PE        */
                            unsigned short OE  :1;      /*    O/E       */
                            unsigned short STOP:1;      /*    STOP      */
                            unsigned short     :1;      /*              */
                            unsigned short CKS :2;      /*    CKS       */
                            }       BIT;                /*              */
                     }          SCSMR;                  /*              */
               char             wk1[2];                 /*              */
               unsigned char    SCBRR;                  /* SCBRR        */
               char             wk2[3];                 /*              */
               union {                                  /* SCSCR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :8;      /*              */
                            unsigned short TIE :1;      /*    TIE       */
                            unsigned short RIE :1;      /*    RIE       */
                            unsigned short TE  :1;      /*    TE        */
                            unsigned short RE  :1;      /*    RE        */
                            unsigned short REIE:1;      /*    REIE      */
                            unsigned short     :1;      /*              */
                            unsigned short CKE :1;      /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3[2];                 /*              */
               unsigned char    SCFTDR;                 /* SCFTDR       */
               char             wk4[3];                 /*              */
               union {                                  /* SCFSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short PERC:4;      /*    PERC      */
                            unsigned short FERC:4;      /*    FERC      */
                            unsigned short ER  :1;      /*    ER        */
                            unsigned short TEND:1;      /*    TEND      */
                            unsigned short TDFE:1;      /*    TDFE      */
                            unsigned short BRK :1;      /*    BRK       */
                            unsigned short FER :1;      /*    FER       */
                            unsigned short PER :1;      /*    PER       */
                            unsigned short RDF :1;      /*    RDF       */
                            unsigned short DR  :1;      /*    DR        */
                            }       BIT;                /*              */
                     }          SCFSR;                  /*              */
               char             wk5[2];                 /*              */
               unsigned char    SCFRDR;                 /* SCFRDR       */
               char             wk6[3];                 /*              */
               union {                                  /* SCFCR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short      :8;     /*              */
                            unsigned short RTRG :2;     /*    RTRG      */
                            unsigned short TTRG :2;     /*    TTRG      */
                            unsigned short MCE  :1;     /*    MCE       */
                            unsigned short TFRST:1;     /*    TFRST     */
                            unsigned short RFRST:1;     /*    RFRST     */
                            unsigned short LOOP :1;     /*    LOOP      */
                            }       BIT;                /*              */
                     }          SCFCR;                  /*              */
               char             wk7[2];                 /*              */
               union {                                  /* SCFDR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short  :3;         /*              */
                            unsigned short T:5;         /*    T         */
                            unsigned short  :3;         /*              */
                            unsigned short R:5;         /*    R         */
                            }       BIT;                /*              */
                     }          SCFDR;                  /*              */
               char             wk8[2];                 /*              */
               union {                                  /* SCSPTR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short       :8;    /*              */
                            unsigned short RTSIO :1;    /*    RTSIO     */
                            unsigned short RTSDT :1;    /*    RTSTD     */
                            unsigned short CTSIO :1;    /*    CTSIO     */
                            unsigned short CTSDT :1;    /*    CTSDT     */
                            unsigned short       :2;    /*              */
                            unsigned short SPB2IO:1;    /*    SPB2IO    */
                            unsigned short SPB2DT:1;    /*    SPB2DT    */
                            }       BIT;                /*              */
                     }          SCSPTR;                 /*              */
               char             wk9[2];                 /*              */
               union {                                  /* SCLSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short     :15;     /*              */
                            unsigned short ORER: 1;     /*    ORER      */
                            }       BIT;                /*              */
                     }          SCLSR;                  /*              */
};                                                      /*              */
struct st_hudi {                                        /* struct H-UDI */
               union {                                  /* SDIR         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit Access  */
                            unsigned short TI:4;        /*    TI        */
                            }       BIT;                /*              */
                     }          SDIR;                   /*              */
               char             wk[6];                  /*              */
               unsigned int     SDDR;                   /* SDDR         */
};                                                      /*              */
#define MMU   (*(volatile struct st_mmu   *)0xFF000000) /* MMU   Address*/
#define CACHE (*(volatile struct st_cache *)0xFF00001C) /* CACHE Address*/
#define EXP   (*(volatile struct st_exp   *)0xFF000020) /* EXP   Address*/
#define UBC   (*(volatile struct st_ubc   *)0xFF000014) /* UBC   Address*/
#define BSC   (*(volatile struct st_bsc   *)0xFF800000) /* BSC   Address*/
#define PORT  (*(volatile struct st_port  *)0xFF80002C) /* PORT  Address*/
#define DMAC  (*(volatile struct st_dmac  *)0xFFA00040) /* DMAC  Address*/
#define DMAC0 (*(volatile struct st_dmac0 *)0xFFA00000) /* DMAC0 Address*/
#define DMAC1 (*(volatile struct st_dmac0 *)0xFFA00010) /* DMAC1 Address*/
#define DMAC2 (*(volatile struct st_dmac2 *)0xFFA00020) /* DMAC2 Address*/
#define DMAC3 (*(volatile struct st_dmac2 *)0xFFA00030) /* DMAC3 Address*/
#define CPG   (*(volatile struct st_cpg   *)0xFFC00000) /* CPG   Address*/
#define STB   (*(volatile struct st_stb   *)0xFFC00004) /* STB   Address*/
#define RTC   (*(volatile struct st_rtc   *)0xFFC80000) /* RTC   Address*/
#define TMU   (*(volatile struct st_tmu   *)0xFFD80000) /* TMU   Address*/
#define TMU0  (*(volatile struct st_tmu0  *)0xFFD80008) /* TMU0  Address*/
#define TMU1  (*(volatile struct st_tmu0  *)0xFFD80014) /* TMU1  Address*/
#define TMU2  (*(volatile struct st_tmu2  *)0xFFD80020) /* TMU2  Address*/
#define INTC  (*(volatile struct st_intc  *)0xFFD00000) /* INTC  Address*/
#define SCI   (*(volatile struct st_sci   *)0xFFE00000) /* SCI   Address*/
#define SMCI  (*(volatile struct st_smci  *)0xFFE00000) /* SMCI  Address*/
#define SCIF  (*(volatile struct st_scif  *)0xFFE80000) /* SCIF  Address*/
#define HUDI  (*(volatile struct st_hudi  *)0xFFF00000) /* H-UDI Address*/
#endif //_IO_DEFINE_H_




