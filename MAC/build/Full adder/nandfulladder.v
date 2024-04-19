module top(a,b,cin,carry,sum);
    input a;
    input b;
    input cin;
    output carry;
    output sum;
    
     wire w1, w2, w3, w4, w5, w6, w7;
     
     //assign sum=a^b^cin;
     //assign carry=((a&b)|(cin&(a^b)));
     nand (w1,a,b);
     nand (w2,a,w1);
     nand (w3,b,w1);
     nand (w4, w3, w2);
     nand (w5, w4, cin);
     nand (w6, w5, w4);
     nand (w7, w5, cin);
     nand (sum, w7, w6);
     nand (carry, w1, w5);
        
endmodule
