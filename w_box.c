/*
	atg - a tiny GUI toolkit for SDL
	Copyright (C) 2012 Edward Cree
	
	See atg.h for license information
	w_box.c: implements the BOX widget
*/
#include "atg.h"
#include "atg_internals.h"

SDL_Surface *atg_render_box(const atg_element *e)
{
	SDL_Surface *screen=SDL_GetVideoSurface();
	if(!screen) return(NULL); /* can't find out display format */
	if(!e) return(NULL);
	atg_box *b=e->elemdata;
	if(!b) return(NULL);
	SDL_Surface **els=malloc(b->nelems*sizeof(SDL_Surface *)), *rv=NULL;
	if(!els) return(NULL);
	for(unsigned int i=0;i<b->nelems;i++)
		els[i]=atg_render_element(b->elems[i]);
	/* Box packing.
		This is all a horrid mess; there must be a cleaner way to do this */
	/* Packing rules: for VERTICAL [HORIZONTAL]
		If we have a height [width], we fill up each column [row] to that size;
		once we get something that doesn't fit, we give up on that column [row]
		and never go back to it.
		If we also have a width [height], we ignore anything that's left once
		it's been exceeded.
		If we only have a width [height] and no height [width], we just treat
		it as a minimum, and stack everything into a single column [row].
		If we have neither, we stack everything into a single column [row] that
		is as wide [tall] as the widest [tallest] element. */
	if(b->flags&ATG_BOX_PACK_VERTICAL)
	{
		if(e->h)
		{
			if(e->w)
			{
				rv=SDL_CreateRGBSurface(SDL_HWSURFACE, e->w, e->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
				SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
				unsigned int y=0, x=0, xmax=0;
				for(unsigned int i=0;i<b->nelems;i++)
				{
					if(x>=e->w) break;
					if(!els[i]) continue;
					if(y+els[i]->h>e->h)
					{
						y=0;
						x=xmax;
					}
					SDL_BlitSurface(els[i], NULL, rv, &(SDL_Rect){.x=x, .y=y});
					b->elems[i]->display=(SDL_Rect){.x=x, .y=y, .w=els[i]->w, .h=els[i]->h};
					y+=els[i]->h;
					if(x+els[i]->w>xmax)
						xmax=x+els[i]->w;
				}
			}
			else
			{
				unsigned int x=0, y=0, xmax=0;
				for(unsigned int i=0;i<b->nelems;i++)
				{
					if(!els[i]) continue;
					if(y+els[i]->h>e->h)
					{
						y=0;
						x=xmax;
					}
					b->elems[i]->display=(SDL_Rect){.x=x, .y=y, .w=els[i]->w, .h=els[i]->h};
					y+=els[i]->h;
					if(x+els[i]->w>xmax)
						xmax=x+els[i]->w;
				}
				rv=SDL_CreateRGBSurface(SDL_HWSURFACE, xmax, e->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
				SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
				for(unsigned int i=0;i<b->nelems;i++)
					if(els[i])
						SDL_BlitSurface(els[i], NULL, rv, &b->elems[i]->display);
			}
		}
		else
		{
			unsigned int x=e->w, y=0;
			for(unsigned int i=0;i<b->nelems;i++)
				if(els[i])
				{
					y+=els[i]->h;
					if((unsigned int)els[i]->w>x) x=els[i]->w;
				}
			rv=SDL_CreateRGBSurface(SDL_HWSURFACE, x, y, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
			SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
			y=0;
			for(unsigned int i=0;i<b->nelems;i++)
				if(els[i])
				{
					SDL_BlitSurface(els[i], NULL, rv, &(SDL_Rect){.x=0, .y=y});
					b->elems[i]->display=(SDL_Rect){.x=0, .y=y, .w=els[i]->w, .h=els[i]->h};
					y+=els[i]->h;
				}
		}
	}
	else
	{
		if(e->w)
		{
			if(e->h)
			{
				rv=SDL_CreateRGBSurface(SDL_HWSURFACE, e->w, e->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
				SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
				unsigned int y=0, x=0, ymax=0;
				for(unsigned int i=0;i<b->nelems;i++)
				{
					if(y>=e->h) break;
					if(!els[i]) continue;
					if(x+els[i]->w>e->w)
					{
						x=0;
						y=ymax;
					}
					SDL_BlitSurface(els[i], NULL, rv, &(SDL_Rect){.x=x, .y=y});
					b->elems[i]->display=(SDL_Rect){.x=x, .y=y, .w=els[i]->w, .h=els[i]->h};
					x+=els[i]->w;
					if(y+els[i]->h>ymax)
						ymax=y+els[i]->h;
				}
			}
			else
			{
				unsigned int x=0, y=0, ymax=0;
				for(unsigned int i=0;i<b->nelems;i++)
				{
					if(!els[i]) continue;
					if(x+els[i]->w>e->w)
					{
						x=0;
						y=ymax;
					}
					b->elems[i]->display=(SDL_Rect){.x=x, .y=y, .w=els[i]->w, .h=els[i]->h};
					x+=els[i]->w;
					if(y+els[i]->h>ymax)
						ymax=y+els[i]->h;
				}
				rv=SDL_CreateRGBSurface(SDL_HWSURFACE, e->w, ymax, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
				SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
				for(unsigned int i=0;i<b->nelems;i++)
					if(els[i])
						SDL_BlitSurface(els[i], NULL, rv, &b->elems[i]->display);
			}
		}
		else
		{
			unsigned int x=0, y=e->h;
			for(unsigned int i=0;i<b->nelems;i++)
				if(els[i])
				{
					x+=els[i]->w;
					if((unsigned int)els[i]->h>y) y=els[i]->h;
				}
			rv=SDL_CreateRGBSurface(SDL_HWSURFACE, x, y, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
			SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->bgcolour.r, b->bgcolour.g, b->bgcolour.b, b->bgcolour.a));
			x=0;
			for(unsigned int i=0;i<b->nelems;i++)
				if(els[i])
				{
					SDL_BlitSurface(els[i], NULL, rv, &(SDL_Rect){.x=x, .y=0});
					b->elems[i]->display=(SDL_Rect){.x=x, .y=0, .w=els[i]->w, .h=els[i]->h};
					x+=els[i]->w;
				}
		}
	}
	for(unsigned int i=0;i<b->nelems;i++)
		SDL_FreeSurface(els[i]);
	free(els);
	return(rv);
}

void atg_click_box(struct atg_event_list *list, struct atg_element *element, SDL_MouseButtonEvent button, unsigned int xoff, unsigned int yoff)
{
	atg_box *b=element->elemdata;
	if(!b->elems) return;
	for(unsigned int i=0;i<b->nelems;i++)
		atg__match_click_recursive(list, b->elems[i], button, xoff+element->display.x, yoff+element->display.y);
}

atg_box *atg_create_box(Uint8 flags, atg_colour bgcolour)
{
	atg_box *rv=malloc(sizeof(atg_box));
	if(rv)
	{
		rv->flags=flags;
		rv->nelems=0;
		rv->elems=NULL;
		rv->bgcolour=bgcolour;
	}
	return(rv);
}

int atg_pack_element(atg_box *box, atg_element *elem)
{
	if(!box) return(1);
	unsigned int n=box->nelems++;
	atg_element **new=realloc(box->elems, box->nelems*sizeof(atg_element *));
	if(new)
	{
		(box->elems=new)[n]=elem;
		return(0);
	}
	box->nelems=n;
	return(1);
}

int atg_pack_box(atg_element *ebox, atg_element *elem)
{
	atg_box *b=ebox->elemdata;
	if(!b) return(1);
	return(atg_pack_element(b, elem));
}

int atg_empty_box(atg_box *box)
{
	if(box)
	{
		for(unsigned int e=0;e<box->nelems;e++)
			atg_free_element(box->elems[e]);
		free(box->elems);
		box->nelems=0;
		box->elems=NULL;
	}
	return(0);
}

atg_box *atg_copy_box_box(const atg_box *b)
{
	atg_box *b2=malloc(sizeof(atg_box));
	if(!b2)
		return(NULL);
	*b2=*b;
	for(unsigned int i=0;i<b->nelems;i++)
		b2->elems[i]=atg_copy_element(b->elems[i]);
	return(b2);
}

atg_element *atg_copy_box(const atg_element *e)
{
	if(!e) return(NULL);
	atg_box *b=e->elemdata;
	if(!b) return(NULL);
	atg_element *rv=malloc(sizeof(atg_element));
	if(!rv) return(NULL);
	*rv=*e;
	if(!(rv->elemdata=atg_copy_box_box(b)))
	{
		free(rv);
		return(NULL);
	}
	return(rv);
}

void atg_free_box(atg_element *e)
{
	if(!e) return;
	atg_box *box=e->elemdata;
	atg_free_box_box(box);
}

void atg_free_box_box(atg_box *box)
{
	atg_empty_box(box);
	free(box);
}

atg_element *atg_create_element_box(Uint8 flags, atg_colour bgcolour)
{
	atg_element *rv=malloc(sizeof(atg_element));
	if(!rv) return(NULL);
	atg_box *b=atg_create_box(flags, bgcolour);
	if(!b)
	{
		free(rv);
		return(NULL);
	}
	rv->w=rv->h=0;
	rv->type="__builtin_box";
	rv->elemdata=b;
	rv->clickable=false;
	rv->hidden=false;
	rv->cache=false;
	rv->cached=NULL;
	rv->userdata=NULL;
	rv->render_callback=atg_render_box;
	rv->match_click_callback=atg_click_box;
	rv->pack_callback=atg_pack_box;
	rv->copy_callback=atg_copy_box;
	rv->free_callback=atg_free_box;
	return(rv);
}
