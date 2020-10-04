#ifndef CODE_SEGTREE_H
#define CODE_SEGTREE_H

#include "../ref/template.hpp"
template <class type>
class PersistantSegTree{
private:
    int Nodes=0,N,n;
    vector<type>seg,lp,rp;
     // persistant seg tree for range sum and point update
    int build(int ss,int se,vector<int>&a)
    {
        int pos=Nodes++;
        if(ss==se)
        {
            seg[pos]=a[ss];
            lp[pos]=-1;
            rp[pos]=-1;
            return pos;
        }
        int mid=(ss+se)/2;
        lp[pos]=build(ss,mid,a);
        rp[pos]=build(mid+1,se,a);
        seg[pos]=seg[lp[pos]]+seg[rp[pos]];
        return pos;
    }
    int update(int root,int ss,int se,int ind,int val){
        if(ind < ss || ind > se)
            return root;
        int pos=Nodes++;
        if(ss == se)
        {
            seg[pos]=val;
            return pos;
        }
        int mid=(ss+se)/2;
        if(ind <=mid)
        {
            lp[pos]=update(lp[root],ss,mid,ind,val);
            rp[pos]=rp[root];
            seg[pos]=seg[lp[pos]]+seg[rp[pos]];
            return pos;
        }
        else
        {
            rp[pos]=update(rp[root],mid+1,se,ind,val);
            lp[pos]=lp[root];
            seg[pos]=seg[lp[pos]]+seg[rp[pos]];
            return pos;
        }
    }
    type query(int pos,int ss,int se,int l,int r)
    {
        if(l <=ss && r>=se)
            return seg[pos];
        if(l > se || r < ss)
            return 0;
        int mid=(ss+se)/2;
        return query(lp[pos],ss,mid,l,r)+query(rp[pos],mid+1,se,l,r);
    }
public:
    PersistantSegTree(vector<int>&a)
    {
        n=a.size();
        N=4*n+1;
        seg.resize(N,0);
        lp.resize(N,-1)
        rp.resize(N,-1);
        build(0,n-1,a);
    }
    int Update(int root,int ind,int val)
    {
        return update(root,0,n-1,ind,val);
    }
    type Query(int root,int l,int r)
    {
        return query(root,0,n-1,l,r);
    }
};


