t_token *get_token(char *line)
{
 t_token *prev;
 t_token *next;
 int  i;
 int  sep;

 prev = NULL;
 next = NULL;
 i = 0;
 _str_trim(line);
 while (line[i])
 {
  sep = ignore_ope(line, i);
  next = next_token(line, &i);
  next->prev = prev;
  if (prev)
   prev->next = next;
  prev = next;
  token_type(next, sep);
  _skip_32(line, &i);
 }
 if (next)
  next->next = NULL;
 while (next && next->prev)
  next = next->prev;
 return (next);
}
