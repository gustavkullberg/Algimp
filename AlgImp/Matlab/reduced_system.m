function [ T_new,q_new ] = reduced_system(T,q,n1,n2,s_prime )
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here

[s,r] = size(T);
T_new=zeros(s_prime,r-1);
q_new=zeros(s_prime,1);
l=1;

for k=1:n1
    for j=n1+1:n2
        T_new(l,:)=T(k,1:r-1)-T(j,1:r-1);
        q_new(l,1)=q(k)-q(j);
        l= l + 1;
    end
end
if (s-n2)>0
    T_new(l:end,:)=T(n2+1:end,1:(r-1));
    q_new(l:end,1)=q(n2+1:end);
end
end

