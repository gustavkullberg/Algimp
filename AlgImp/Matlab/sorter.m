function [ T_sort, q_sort ] = sorter(T,q,n1,n2,tol)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

    T_sort=zeros(size(T));
    q_sort=zeros(size(q));
    pos_count=1;
    neg_count=n1+1;
    zero_count=n2+1;
    [s,r]=size(T);
    
    for i=1:s
        if T(i,r)>0 && abs(T(i,r))>tol
           T_sort(pos_count,:) = T(i,:);
           q_sort(pos_count) = q(i);
           pos_count = pos_count + 1;
        elseif T(i,r)<0 && abs(T(i,r))>tol
            T_sort(neg_count,:) = T(i,:);
            q_sort(neg_count) = q(i);
            neg_count = neg_count + 1;
        else
            T_sort(zero_count,:) = T(i,:);
            q_sort(zero_count) = q(i);
            zero_count = zero_count + 1;
        end
    end
            
       
end

