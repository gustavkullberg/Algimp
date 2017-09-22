function [ a] = MS( Br,br,q,n2)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
if n2==size(q,1);
    if br>Br
        a=0;
    else
        a=1;
    end
    return;
end

q=q(n2+1:end);
if br>Br || min(q)<0
    a=0;
else
    a=1;
end
 
end