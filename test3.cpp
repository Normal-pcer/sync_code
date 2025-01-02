# include <stdio.h>
# include <stdlib.h>

int arr[100005];
int n,m;

struct Tree
{
    int l,r;
    long long sum;
    long long tag;
    struct Tree* chi[2];
};

void add_tag(struct Tree* node,int w)
{
    if (node != NULL)
    {
        node->sum += (node->r-node->l+1) * w;
        node->tag += w;
    }
    return ;
}

void push_down(struct Tree* node)
{
    if (node->tag)
    {
        add_tag(node->chi[0],node->tag);
        add_tag(node->chi[1],node->tag);
        node->tag = 0;
    }
    return ;
}

struct Tree* ini(int l,int r)
{
    struct Tree* tmp = (struct Tree*) malloc (sizeof(struct Tree));
    tmp->chi[0] = tmp->chi[1] = NULL;
    tmp->l = l;
    tmp->r = r;
    tmp->sum = 0;
    tmp->tag = 0;
    return tmp;
}

void push_up(struct Tree* node)
{
    node->sum = node->chi[0]->sum + node->chi[1]->sum;
    return ;
}

void build(struct Tree* node,int l,int r)
{
    // printf ("sum:%lld l:%d r:%d mid:%d\n",node->sum,l,r,(l+r)/2);
    if (l == r)
    {
        node->sum = arr[l];
        return ;
    }
    // exit(-1);
    int mid = (l + r) / 2;

    node->chi[0] = ini(l,mid);
    node->chi[1] = ini(mid+1,r);

    build(node->chi[0],l,mid);
    build(node->chi[1],mid+1,r);

    push_up(node);

    return ;
}

void update(struct Tree* node,int tl,int tr,int w)
{
    if (tl <= node->l && node->r <= tr)
    {
        add_tag(node,w);
        return ;
    }

    push_down(node);

    int mid = (node->l + node->r) / 2;

    if (mid >= tl)
    {
        update(node->chi[0],tl,tr,w);
    }
    if (mid < tr)
    {
        update(node->chi[1],tl,tr,w);
    }

    push_up(node);

    return ;
}

long long query(struct Tree* node,int tl,int tr)
{
    if (tl <= node->l && node->r <= tr)
    {
        return node->sum;
    }

    push_down(node);

    int mid = (node->l + node->r) / 2;
    long long res=0;

    if (mid >= tl)
    {
        res += query(node->chi[0],tl,tr);
    }
    if (mid < tr)
    {
        res += query(node->chi[1],tl,tr);
    }
    push_up(node);

    return res;
}

void destroy(struct Tree* node) {
    if (node->chi[0])  destroy(node->chi[0]);
    if (node->chi[1])  destroy(node->chi[1]);
    free(node);
}

int main (void)
{
    scanf ("%d %d",&n,&m);

    for (int i=1;i<=n;i++)
    {
        scanf ("%d",&arr[i]);
    }

    struct Tree* root = ini(1,n);

    // printf ("%d\n",n);

    build(root,1,n);

    for (int i=0;i<m;i++)
    {
        int opt,x,y;
        scanf ("%d %d %d",&opt,&x,&y);
        if (opt == 1)
        {
            int k;
            scanf ("%d",&k);
            update(root,x,y,k);
        }
        else
        {
            printf ("%lld\n",query(root,x,y));
        }
    }
    destroy(root);
    return 0;
}