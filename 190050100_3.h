#include <algorithm>
#include<math.h>

struct point
{
    int x,y;
};

class quad_tree{
	point top,bottom;
    int height,intensity;
    quad_tree *p[4];
    public:	
	void group()
	{
		if(p[0]==NULL)return;
        if( p[0]->p[0]!=NULL &&p[1]->p[0]!=NULL && p[2]->p[0]!=NULL && p[3]->p[0]!=NULL)
        {
        if((p[0] -> intensity)==(p[1] -> intensity) && (p[0] -> intensity)==(p[3] -> intensity) && (p[0] -> intensity)==(p[2] -> intensity)){
            int k = (p[0] -> intensity);            
            intensity = k;
            for(int i=0;i<4;i++){
                delete p[i];
                p[i]= NULL;
            }
            }return;
        }return;

	}
	
	long int  black(){
        if(p[0]==NULL){
            if(intensity == 1)return pow(2,2*height);
            if(intensity == 0)return 0;
            }
        return p[0] -> black() + p[1] -> black() + p[2] -> black() + p[3] -> black();
    }

    quad_tree(int x1,int y1,int x2,int y2,int x,int n){
        top.x = x1;
        top.y = y1;
        bottom.x = x2;
        bottom.y = y2;
        intensity = x;
        height = n;
        for(int i=0;i<4;i++)p[i]=NULL;
    }
    
	
	quad_tree(int n){
        top.x = 0;
        top.y = 0;
        bottom.x = pow(2,n)-1;
        bottom.y = pow(2,n)-1;
        height = n;
        intensity = 0;
        for(int i=0;i<4;i++)p[i]=NULL;
    }
	~quad_tree(){
        intensity = 0;
        height = 0;
        top.x = 0;
        top.y = 0;
        bottom.x =0;
        bottom.y =0;
        for(int i=0;i<4;i++)
        {delete p[i];p[i]=NULL;}
    }
	quad_tree(quad_tree const &Q){
        intensity = Q.intensity;
        height = Q.height;
        top.x = Q.top.x;
        top.y = Q.top.y;
        bottom.x = Q.bottom.x;
        bottom.y = Q.bottom.y;
        for(int i=0;i<4;i++)
            if(Q.p[i])p[i] = new quad_tree(*Q.p[i]);
    }
     void set(int x1,int y1,int x2,int y2,int b){    
        if(top.x==x1 && top.y==y1 && bottom.x == x2 && bottom.y == y2){
            intensity =b;
            this -> group();
            for(int i=0;i<4;i++){delete p[i];p[i]=NULL;}
            return;
        }
        point mid;
            mid.x=(top.x+bottom.x)/2;
            mid.y=(top.y+bottom.y)/2;
        if(!p[0]){
            if (intensity==b)return;
            p[0] = new quad_tree(top.x,top.y,mid.x,mid.y,intensity,height-1);
            p[1] = new quad_tree(top.x,mid.y+1,mid.x,bottom.y,intensity,height-1);
            p[2] = new quad_tree(mid.x+1,top.y,bottom.x,mid.y,intensity,height-1);
            p[3] = new quad_tree(mid.x+1,mid.y+1,bottom.x,bottom.y,intensity,height-1);
        }
        for(int i=0;i<4;i++)
            if(x1>=p[i]->top.x && x2<=p[i]->bottom.x && y1>=p[i]->top.y && y2<=p[i]->bottom.y){
                p[i]->set(x1,y1,x2,y2,b);
                this -> group();
                return;
            }
        if(x2<=mid.x){
            p[0] -> set(x1,y1,x2,mid.y,b);
            p[1] -> set(x1,mid.y+1,x2,y2,b);
        }    
        else if(x1>mid.x){
            p[2] -> set(x1,y1,x2,mid.y,b);
            p[3] -> set(x1,mid.y+1,x2,y2,b);            
        }
        else if(y2<=mid.y){
            p[0] -> set(x1,y1,mid.x,y2,b);
            p[2] -> set(mid.x+1,y1,x2,y2,b);
        }
        else if(y1>mid.y){
            p[1] -> set(x1,y1,mid.x,y2,b);
            p[3] -> set(mid.x+ 1,y1,x2,y2,b);            
        }
        else{
            p[0] -> set(x1,y1,mid.x,mid.y,b);            
            p[1] -> set(x1,mid.y+ 1,mid.x,y2,b);
            p[2] -> set(mid.x+1,y1,x2,mid.y,b);
            p[3] -> set(mid.x+1,mid.y+1,x2,y2,b);
        }
        this -> group();
        return;
        
    }
    int get(int x1,int y1){        
        if(p[0]==NULL)return intensity;
        if(x1<=(top.x+bottom.x)/2){
            if(y1<=(top.y+bottom.y)/2)return p[0] -> get(x1,y1);
            return p[1] -> get(x1,y1);
        }
        if(y1<=(top.y+bottom.y)/2)return p[2] -> get(x1,y1);
        return p[3] -> get(x1,y1);
    }
	
	int size(){
        return height;
    }
	void overlap(quad_tree const &Q)
	{
		if(p[0])
		{
			if(Q.p[0]==NULL )
			{
				if(Q.intensity==0) return;
				else
				{		
					intensity = 1;
		        for(int i=0;i<4;i++){delete p[i];p[i]=NULL; }			
					return;
				}
			}
			else
			{
				for(int i=0;i<4;i++)p[i]->overlap(*Q.p[i]);
				this->group();
				return;
			}
		}
		
		if(intensity==1)
			return;
		
			if(Q.p[0]==NULL){
			
				intensity = Q.intensity;
				return;
			}
			
				for(int i=0;i<4;i++)p[i] = new quad_tree(*Q.p[i]);
              	
		
	}
	void intersect(quad_tree &Q)
	{
		if(p[0])
		{
			if(Q.p[0]==NULL)
			{
				if(Q.intensity==1) return;
				else{		
					intensity = 0;
		        for(int i=0;i<4;i++){delete p[i];p[i]=NULL; }
					return;
                }
			}
			else
			{
				for(int i=0;i<4;i++)p[i]->intersect(*Q.p[i]);
				this->group();
				return;
			}
		}
		
		if(intensity==0)
			return;
		
			if(Q.p[0]==NULL && Q.p[1]==NULL && Q.p[2]==NULL && Q.p[3]==NULL)
			{
				intensity = Q.intensity;
				return;
			}
			
		    for(int i=0;i<4;i++)p[i] = new quad_tree(*Q.p[i]);
		
		
	}
	void complement()
	{
		 if(!p[0]){
            intensity = 1-intensity;
            return;
        }
        for(int i=0;i<4;i++)     
            p[i] -> complement();
	}
    void resize(int m){
        if(m == height)return;
        else if(m>height){
            height = m;
            int length = pow(2,m);
            bottom.x = top.x + length - 1;
            bottom.y = top.y + length - 1;
            if(!p[0])return;
            for(int i=0;i<4;i++){
                if(i<2) p[i]->top.x=top.x;
                else p[i]->top.x=top.x+length/2;

                if(i%2==0) p[i]->top.y=top.y;
                else p[i]->top.y=top.y+length/2;
            }
            for(int i=0;i<4;i++)
                p[i] -> resize(m-1);
        }
        if(m == 0){
            if(this -> black()>=pow(2,2*height - 1))this -> intensity = 1;
            else this -> intensity = 0;
            height = 0;
            bottom.x = top.x;bottom.y = top.y;
            for(int i=0;i<4;i++){
                delete p[i];
                p[i] =NULL ;
            }           
        }
        height = m;
        int length = pow(2,m);
        bottom.y = top.y + length - 1;
        bottom.x = top.x + length - 1;
        if(!p[0])return;
        for(int i=0;i<4;i++){
            if(i<2) p[i]->top.x=top.x;
            else p[i]->top.x=top.x+length/2;

            if(i%2==0) p[i]->top.y=top.y;
            else p[i]->top.y=top.y+length/2;
        }
        for(int i=0;i<4;i++)
            p[i] -> resize(m-1);
    }
    void extract(int x1,int y1,int m){
        if (height == m) return;
        if(!p[0]){height = m; return;}
        if (m == 0) {
            intensity = this->get(x1,y1);
            height= 0;
            for(int i=0;i<4;i++)this->p[i]= NULL;
            return;
        }




        quad_tree *pq[4];
        for(int i=0;i<4;i++) pq[i]= new quad_tree(*this);
        pq[0] -> extract(x1, y1, m - 1);
        pq[1] -> extract(x1 + pow(2, m - 1), y1, m - 1);
        pq[2] -> extract(x1 + pow(2, m - 1),  y1 + pow(2, m - 1), m - 1);
        pq[3] -> extract(x1,  y1 + pow(2, m - 1), m - 1);
        p[0] = pq[0];
        p[1] = pq[1];
        p[2] = pq[2];
        p[3] = pq[3];
        height = m;
        this -> group();
        return;
    }
  
};
