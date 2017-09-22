function  a  = FME3

format long
matrices
T = A1;
q = C1;
%lc = 0;
tol=0.001;

while 1==1
   [s,r]=size(T);
   n1=0;
   n2=0;
   
   for i=1:s
        if T(i,r)>0 && abs(T(i,r))>tol 
            n1=n1+1;
            n2=n2+1;
        end
        if T(i,r)<0 && abs(T(i,r))>tol
            n2=n2+1;
        end 
   end
  
   [T, q] = sorter(T,q,n1,n2,tol);
   
   [T, q] = divider(T,q,n2);
   
   if r==1
       if n2>n1
           br=max(q(n1+1:n2));
       else
           br=-inf;
       end
       if n1>0
           Br=min(q(1:n1));
       else
           Br=inf;
       end
       
       a = MS( Br,br,q,n2);
       return;
   end
    
   s_prime = s - n2 + n1*(n2-n1);
   if s_prime == 0
       a = 1;
       return;
   end
   
   [T,q] = reduced_system(T,q,n1,n2,s_prime);
end


end