// return 0 is bad sequence, otherwise returns sequence value


using System;

public static class Utility
{
    const int WILD = 1;                                               //*************Wild card score is atleast 1*********************
    static int validateSequence(int[] cards)
    {
        if (cards.Length < 2) return 0;                               //need at least 2 to make a valid sequence

        int val = 0;

        /// first card is wild
        if (cards[0] != WILD)
        {
            val = cards[0];                           
        }
        else
        {
            val = cards[1];                                         
        }


        if (val == WILD)                                           		//************* at least first two are wild
        {
            int w;                                                      //********** Loop finds first WILD card or in this case, card with value 1********************

            for (w = 2; val == WILD && w < cards.Length; w++) 			//traverse through sequence and find first non-wild
            {
                val = cards[w];
            }

                                                                        //***************If val is still WILD*************
            if (val == WILD)
            {
                return (int)Math.Pow(2, cards.Length);                 //entire sequence is wild //(good sequence)
            }

            //***************If val is not WILD************* All are non wild except first two
            val >>= (w - 2);                                            //************divide by 2 (length - 2 times)
            if (val < 2) return 0;                                 		// sequence must start at at least a value of 2
        }

        //validate sequence and return value
        if ((cards[0] == val || cards[0] == WILD) && (cards[1] == val || cards[1] == WILD))
        {
            for (int i = 2; i < cards.Length; i++)
            {
                val <<= 1;

                //************if not a wild or same as last number sum score earned =0
                if (cards[i] != val && cards[i] != WILD) return 0; 
            }

            //************ last score * 2; // return double of merged sequence
            return val << 1;
        }

        return 0;                                                   	//bad sequence
    }
}
