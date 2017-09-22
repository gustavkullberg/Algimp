function [ T_divided, q_divided ] = divider(T,q,n2)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here

T_divided = zeros(size(T));
q_divided = zeros(size(q));
[s,r] = size(T);

for i=1:n2
   t = T(i,r);
   T_divided(i,:) = T(i,:)/t;
   q_divided(i) = q(i)/t;
end
if (s-n2)>0
   T_divided(n2+1:end,:) = T(n2+1:end,:);
   q_divided(n2+1:end) = q(n2+1:end);
end
end

