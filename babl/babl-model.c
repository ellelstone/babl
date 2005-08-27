/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005, Øyvind Kolås.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "babl-internal.h"
#include <string.h>
#include <stdarg.h>
#include "babl-db.h"


static int 
each_babl_model_destroy (Babl *babl,
                         void         *data)
{
  babl_free (babl->model.from);
  babl_free (babl->model.to);
  babl_free (babl);
  return 0;  /* continue iterating */
}

static Babl *
model_new (const char     *name,
           int             id,
           int             components,
           BablComponent **component)
{
  Babl *babl;

  babl                   = babl_malloc (sizeof (BablModel) +
                                        sizeof (BablComponent*) * (components) +
                                        strlen (name) + 1);
  babl->model.component = ((void*)babl) + sizeof (BablModel);
  babl->instance.name   = ((void*)babl->model.component) + sizeof (BablComponent*) * (components);
  
  babl->class_type       = BABL_MODEL;
  babl->instance.id      = id;
  babl->model.components = components;
  strcpy (babl->instance.name, name);
  memcpy (babl->model.component, component, sizeof (BablComponent*)*components);

  babl->model.from         = NULL;
  babl->model.to           = NULL;
  return babl;
}

Babl *
babl_model_new (const char *name,
                       ...)
{
  va_list        varg;
  Babl          *babl;
  int            id     = 0;
  int            components  = 0;
  BablComponent *component [BABL_MAX_COMPONENTS];
  const char    *arg=name;

  va_start (varg, name);

  
  while (1)
    {
      arg = va_arg (varg, char *);
      if (!arg)
        break;


      if (BABL_IS_BABL (arg))
        {
          Babl *babl = (Babl*)arg;

          switch (babl->class_type)
            {
              case BABL_COMPONENT:
                component [components] = (BablComponent*) babl;
                components++;

                if (components>=BABL_MAX_COMPONENTS)
                  {
                    babl_log ("maximum number of components (%i) exceeded for %s",
                      BABL_MAX_COMPONENTS, name);
                  }
                break;
              case BABL_MODEL:
                  babl_log ("submodels not handled yet");
                  break;
              case BABL_TYPE:
              case BABL_SAMPLING:
              case BABL_INSTANCE:
              case BABL_FORMAT:


              case BABL_CONVERSION:
              case BABL_CONVERSION_TYPE:
              case BABL_CONVERSION_TYPE_PLANAR:
              case BABL_CONVERSION_MODEL_PLANAR:
              case BABL_CONVERSION_FORMAT:
              case BABL_CONVERSION_FORMAT_PLANAR:
              case BABL_FISH:
              case BABL_FISH_REFERENCE:
              case BABL_IMAGE:
                babl_log ("%s unexpected", babl_class_name (babl->class_type));
                break;
              case BABL_SKY: /* shut up compiler */
                break;
            }
        }
      /* if we didn't point to a babl, we assume arguments to be strings */
      else if (!strcmp (arg, "id"))
        {
          id = va_arg (varg, int);
        }
      
      else
        {
          babl_log ("unhandled parameter '%s' for babl_model '%s'", arg, name);
          exit (-1);
        }
    }
    
  va_end   (varg);

  babl = model_new (name, id, components, component);
  
  if (db_insert (babl) == babl)
    {
      return babl;
    }
  else
    {
      each_babl_model_destroy (babl, NULL);
      return NULL;
    }
}

BABL_CLASS_TEMPLATE (babl_model)
