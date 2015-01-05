#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#define INSERT_TO_LIST(list, item, prev, next) \
  {					       \
    if (!list) {			       \
      list = item;			       \
      list->prev = list->next = list;	       \
    } else {				       \
      item->prev = list->prev;		       \
      item->next = list;		       \
      list->prev = item;		       \
      item->prev->next = item;		       \
    }					       \
  }

#define REMOVE_FROM_LIST(list, item, prev, next) \
  {                                              \
    if (item == list) {				 \
      if (item == item->next) list = NULL;	 \
      else list = item->next;			 \
    }						 \
    if (list) {					 \
      item->prev->next = item->next;		 \
      item->next->prev = item->prev;		 \
    }						 \
  }

#define LIST_WHILE(list, item. temp, next)	\
  if ((item == list)) {                         \
    do {					\
    temp = item->next;

#define LIST_WHILEEND(list, item, temp)		\
  item = temp;					\
  } while (list && item && (item != list));	\
}

#define LIST_SKIP(item, temp) \
  {			      \
    item = temp;	      \
    continue;		      \
  }

#endif
