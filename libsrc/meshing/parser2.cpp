#include <mystdlib.h>
#include "meshing.hpp"

#ifdef WIN32
#define COMMASIGN ':'
#else
#define COMMASIGN ','
#endif


namespace netgen
{


void LoadMatrixLine (istream & ist, DenseMatrix & m, int line)
{
  char ch;
  int pnum;
  float f;

  ist >> ch;
  PrintMessage (3, "netrule :: LoadMatrixLine, testline 1 ", ch);

  while (ch != '}')
    {

      ist.putback (ch);
      ist >> f;
      ist >> ch;
      ist >> pnum;
  PrintMessage (3, "netrule :: LoadMatrixLine, testline 2 ", ch);
  PrintMessage (3, "netrule :: LoadMatrixLine, testline 3 ", std::to_string(f));

      if (ch == 'x' || ch == 'X')
	m.Elem(line, 2 * pnum - 1) = f;
      if (ch == 'y' || ch == 'Y')
	m.Elem(line, 2 * pnum) = f;

      ist >> ch;
      if (ch == COMMASIGN)
	ist >> ch;
    }
}


void netrule :: LoadRule (istream & ist)
{
	
  PrintMessage (3, "netrule :: LoadRule, testline 1 ");

  char buf[256];
  char ch;
  Point<2> p;
  INDEX_2 lin;
  int i, j;
  DenseMatrix tempoldutonewu(20, 20), tempoldutofreearea(20, 20),
    tempoldutofreearealimit(20, 20);
  PrintMessage (3, "netrule :: LoadRule, testline 2");

  tempoldutonewu = 0;
  tempoldutofreearea = 0;
  tempoldutofreearealimit = 0;
  PrintMessage (3, "netrule :: LoadRule, testline 3");

  noldp = 0;
  noldl = 0;
  PrintMessage (3, "netrule :: LoadRule, testline 4");

  ist.get (buf, sizeof(buf), '"');
  ist.get (ch);
  ist.get (buf, sizeof(buf), '"');
  ist.get (ch);
  PrintMessage (3, "netrule :: LoadRule, testline 5");

  // if(name != NULL) 
  delete [] name;
  name = new char[strlen (buf) + 1];
  strcpy (name, buf);
  //(*testout) << "name " << name << endl;
  //  (*mycout) << "Rule " << name << " found." << endl;
  PrintMessage (3, "netrule :: LoadRule, testline 6 ", name);

  do
    {
      ist >> buf;
  PrintMessage (3, "netrule :: LoadRule, testline 5", buf);

      //(*testout) << "buf " << buf << endl;

      if (strcmp (buf, "quality") == 0)

	{
		  PrintMessage (3, "netrule :: LoadRule, testline 5 quality");

	  ist >> quality;
	}

      else if (strcmp (buf, "mappoints") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints");

	  while (ch == '(')
	    {
	      ist >> p[0];
	      ist >> ch;    // ','
	      ist >> p[1];
	      ist >> ch;    // ')'
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints a");

	      points.Append (p);
	      noldp++;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints b");

	      tolerances.SetSize (noldp);
	      tolerances.Elem(noldp).f1 = 1.0;
	      tolerances.Elem(noldp).f2 = 0;
	      tolerances.Elem(noldp).f3 = 1.0;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints c");

	      ist >> ch;
	      while (ch != ';')
		{
		  if (ch == '{')
		    {
		      ist >> tolerances.Elem(noldp).f1;
		      ist >> ch;  // ','
		      ist >> tolerances.Elem(noldp).f2;
		      ist >> ch;  // ','
		      ist >> tolerances.Elem(noldp).f3;
		      ist >> ch;  // '}'
		    }
		  else if (ch == 'd')
		    {
		      //            delpoints.Append (noldp);
		      ist >> ch; // 'e'
		      ist >> ch; // 'l'
		    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints d");

		  ist >> ch;
		}

	      ist >> ch;
	    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 mappoints e");

	  ist.putback (ch);
	}


      else if (strcmp (buf, "maplines") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 maplines");

	  while (ch == '(')
	    {
	      ist >> lin.I1();
	      ist >> ch;    // ','
	      ist >> lin.I2();
	      ist >> ch;    // ')'

		  PrintMessage (3, "netrule :: LoadRule, testline 5 maplines a");

	      //(*testout) << "read line " << lin.I1() << " " << lin.I2() << endl;
	      lines.Append (lin);
	      linevecs.Append (points.Get(lin.I2()) - points.Get(lin.I1()));
	      noldl++;
	      linetolerances.SetSize (noldl);
	      linetolerances.Elem(noldl).f1 = 0;
	      linetolerances.Elem(noldl).f2 = 0;
	      linetolerances.Elem(noldl).f3 = 0;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 maplines b");

	      //(*testout) << "mapl1" << endl; 
	      ist >> ch;
	      while (ch != ';')
		{
		  //(*testout) << "working on character \""<<ch<<"\""<< endl;
		  if (ch == '{')
		    {
		      ist >> linetolerances.Elem(noldl).f1;
		      ist >> ch;  // ','
		      ist >> linetolerances.Elem(noldl).f2;
		      ist >> ch;  // ','
		      ist >> linetolerances.Elem(noldl).f3;
		      ist >> ch;  // '}'
		    }
		  else if (ch == 'd')
		    {
		      dellines.Append (noldl);
		      ist >> ch; // 'e'
		      ist >> ch; // 'l'
		      //(*testout) << "read del" << endl;
		    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 maplines c");

		  ist >> ch;
		  //(*testout) << "read character \""<<ch<<"\""<< endl;
		}

	      ist >> ch;
	      //(*testout) << "read next character \""<<ch<<"\""<< endl;
	    }
	  
		  PrintMessage (3, "netrule :: LoadRule, testline 5 maplines d");

	  ist.putback (ch);
	}

      else if (strcmp (buf, "newpoints") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newpoints");
	  while (ch == '(')
	    {
	      ist >> p[0];
	      ist >> ch;    // ','
	      ist >> p[1];
	      ist >> ch;    // ')'

	      points.Append (p);
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newpoints a");

	      ist >> ch;
	      while (ch != ';')
		{
		  if (ch == '{')
		    {
		      LoadMatrixLine (ist, tempoldutonewu,
				      2 * (points.Size()-noldp) - 1);
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newpoints b");

		      ist >> ch; // '{'
		      LoadMatrixLine (ist, tempoldutonewu,
				      2 * (points.Size()-noldp));
		    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newpoints c");

		  ist >> ch;
		}

	      ist >> ch;
	    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newpoints d");

	  ist.putback (ch);
	}

      else if (strcmp (buf, "newlines") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newlines");

	  while (ch == '(')
	    {
	      ist >> lin.I1();
	      ist >> ch;    // ','
	      ist >> lin.I2();
	      ist >> ch;    // ')'
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newlines a");

	      lines.Append (lin);
	      linevecs.Append (points.Get(lin.I2()) - points.Get(lin.I1()));

	      ist >> ch;
	      while (ch != ';')
		{
		  ist >> ch;
		}

	      ist >> ch;
	    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 newlines b");

	  ist.putback (ch);
	}

      else if (strcmp (buf, "freearea") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea");
	  while (ch == '(')
	    {
	      ist >> p[0];
	      ist >> ch;    // ','
	      ist >> p[1];
	      ist >> ch;    // ')'
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea a");

	      freezone.Append (p);
	      freezonelimit.Append (p);

	      ist >> ch;
	      while (ch != ';')
		{
		  if (ch == '{')
		    {
		      LoadMatrixLine (ist, tempoldutofreearea,
				      2 * freezone.Size() - 1);
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea b");

		      ist >> ch; // '{'
		      LoadMatrixLine (ist, tempoldutofreearea,
				      2 * freezone.Size());
		    }

		  ist >> ch;
		}
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea c");

	      ist >> ch;
	    }

	  for (i = 1; i <= tempoldutofreearealimit.Height(); i++)
	    for (j = 1; j <= tempoldutofreearealimit.Width(); j++)
	      tempoldutofreearealimit.Elem(i,j) =
		tempoldutofreearea.Elem(i,j);

		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea d");

	  ist.putback (ch);
	}    
      else if (strcmp (buf, "freearea2") == 0)
	{
	  ist >> ch;
	  int freepi = 0;
	  tempoldutofreearealimit = 0;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea2");
	  while (ch == '(')
	    {
	      freepi++;

	      ist >> p[0];
	      ist >> ch;    // ','
	      ist >> p[1];
	      ist >> ch;    // ')'
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea a");

	      freezonelimit.Elem(freepi) = p;
	  
	      ist >> ch;
	      while (ch != ';')
		{
		  if (ch == '{')
		    {
		      LoadMatrixLine (ist, tempoldutofreearealimit,
				      2 * freepi - 1);

		      ist >> ch; // '{'
		      LoadMatrixLine (ist, tempoldutofreearealimit,
				      2 * freepi);
		    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea b");

		  ist >> ch;
		}
	  
	      ist >> ch;
	    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 freearea c");

	  ist.putback (ch);
	}

      else if (strcmp (buf, "elements") == 0)
	{
	  ist >> ch;
		  PrintMessage (3, "netrule :: LoadRule, testline 5 elements");

	  while (ch == '(')
	    {
	      elements.Append (Element2d(TRIG));

	      ist >> elements.Last().PNum(1);
	      ist >> ch;    // ','
	  		  PrintMessage (3, "netrule :: LoadRule, testline 5 elements a");

	      if (ch == COMMASIGN)
		{
		  ist >> elements.Last().PNum(2);
		  ist >> ch;    // ','
		}
	      if (ch == COMMASIGN)
		{
					  PrintMessage (3, "netrule :: LoadRule, testline 5 elements b");

		  ist >> elements.Last().PNum(3);
		  ist >> ch;    // ','
		}
	      if (ch == COMMASIGN)
		{
					  PrintMessage (3, "netrule :: LoadRule, testline 5 elements c");

		  elements.Last().SetType (QUAD);
		  ist >> elements.Last().PNum(4);
		  ist >> ch;    // ','
		  
		  // const Element2d & el = elements.Last();
		  /*
		  orientations.Append (threeint(el.PNum(1), el.PNum(2), el.PNum(3)));
		  orientations.Append (threeint(el.PNum(2), el.PNum(3), el.PNum(4)));
		  orientations.Append (threeint(el.PNum(3), el.PNum(4), el.PNum(1)));
		  orientations.Append (threeint(el.PNum(4), el.PNum(1), el.PNum(2)));
		  */
		}

	      ist >> ch;
	      while (ch != ';')
		{
		  ist >> ch;
		}

	      ist >> ch;
	    }
		  PrintMessage (3, "netrule :: LoadRule, testline 5 elements d");

	  ist.putback (ch);
	}

      else if (strcmp (buf, "orientations") == 0)

	{
	  ist >> ch;

	  while (ch == '(')
	    {
	      //        threeint a = threeint();
	      orientations.Append (threeint());

	      ist >> orientations.Last().i1;
	      ist >> ch;    // ','
	      ist >> orientations.Last().i2;
	      ist >> ch;    // ','
	      ist >> orientations.Last().i3;
	      ist >> ch;    // ','

	      ist >> ch;
	      while (ch != ';')
		{
		  ist >> ch;
		}

	      ist >> ch;
	    }

	  ist.putback (ch);
	}

      else if (strcmp (buf, "endrule") != 0)
	{
	  PrintSysError ("Parser error, unknown token ", buf);
	}
    }
  while (!ist.eof() && strcmp (buf, "endrule") != 0);
		  PrintMessage (3, "netrule :: LoadRule, testline 6");

  oldutonewu.SetSize (2 * (points.Size() - noldp), 2 * noldp);
  oldutofreearea.SetSize (2 * freezone.Size(), 2 * noldp);
  oldutofreearealimit.SetSize (2 * freezone.Size(), 2 * noldp);
		  PrintMessage (3, "netrule :: LoadRule, testline 7");

  for (i = 1; i <= oldutonewu.Height(); i++)
    for (j = 1; j <= oldutonewu.Width(); j++)
      oldutonewu.Elem(i, j) = tempoldutonewu.Elem(i, j);

  for (i = 1; i <= oldutofreearea.Height(); i++)
    for (j = 1; j <= oldutofreearea.Width(); j++)
      oldutofreearea.Elem(i, j) = tempoldutofreearea.Elem(i, j);
		  PrintMessage (3, "netrule :: LoadRule, testline 8");

  for (i = 1; i <= oldutofreearea.Height(); i++)
    for (j = 1; j <= oldutofreearea.Width(); j++)
      oldutofreearealimit.Elem(i, j) = tempoldutofreearealimit.Elem(i, j);

  freesetinequ.SetSize (freezone.Size());

		  PrintMessage (3, "netrule :: LoadRule, testline 9");

  {
    char ok;
    int minn;
    NgArray<int> pnearness (noldp);

    for (i = 1; i <= pnearness.Size(); i++)
      pnearness.Elem(i) = 1000;

    for (j = 1; j <= 2; j++)
      pnearness.Elem(GetPointNr (1, j)) = 0;
		  PrintMessage (3, "netrule :: LoadRule, testline 10");

    do
      {
	ok = 1;

	for (i = 1; i <= noldl; i++)
	  {
	    minn = 1000;
	    for (j = 1; j <= 2; j++)
	      minn = min2 (minn, pnearness.Get(GetPointNr (i, j)));

	    for (j = 1; j <= 2; j++)
	      if (pnearness.Get(GetPointNr (i, j)) > minn+1)
		{
		  ok = 0;
		  pnearness.Elem(GetPointNr (i, j)) = minn+1;
		}
	  }
      }
    while (!ok);
		  PrintMessage (3, "netrule :: LoadRule, testline 11");

    lnearness.SetSize (noldl);

    for (i = 1; i <= noldl; i++)
      {
	lnearness.Elem(i) = 0;
	for (j = 1; j <= 2; j++)
	  lnearness.Elem(i) += pnearness.Get(GetPointNr (i, j));
      }
  }
		  PrintMessage (3, "netrule :: LoadRule, testline 12");

  oldutofreearea_i.SetSize (10);
  freezone_i.SetSize (10);
		  PrintMessage (3, "netrule :: LoadRule, testline 13");

  for (i = 0; i < oldutofreearea_i.Size(); i++)
    {
      double lam1 = 1.0/(i+1);
		  PrintMessage (3, "netrule :: LoadRule, testline 13 a ");
		  PrintMessage (3, std::to_string(oldutofreearea.Height()), std::to_string(oldutofreearea.Width()));

      oldutofreearea_i[i] = new DenseMatrix (oldutofreearea.Height(), oldutofreearea.Width());
      DenseMatrix & mati = *oldutofreearea_i[i];
      for (j = 0; j < oldutofreearea.Height(); j++)
	for (int k = 0; k < oldutofreearea.Width(); k++)
	  mati(j,k) = lam1 * oldutofreearea(j,k) + (1 - lam1) * oldutofreearealimit(j,k);

      freezone_i[i] = new NgArray<Point<2>> (freezone.Size());
      auto& fzi = *freezone_i[i];
      for (int j = 0; j < freezone.Size(); j++)
	fzi[j] = freezonelimit[j] + lam1 * (freezone[j] - freezonelimit[j]);
    }
			  PrintMessage (3, "netrule :: LoadRule, testline 14");

}




extern const char * triarules[];
extern const char * quadrules[];

void Meshing2 :: LoadRules (const char * filename, bool quad)
{
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 1 ");
  
  char buf[256];
  istream * ist;
  //char *tr1 = NULL;
  string tr1;

  /*
  ifstream ist (filename);
  if (!ist.good())
    {
      cerr << "Rule description file " << filename << " not found" << endl;
      exit (1);
    }
  */


  if (filename)
    {
      //      (*mycout) << "rule-filename = " << filename << endl;
      ist = new ifstream (filename);
    }
  else 
    {
      /* connect tetrules to one string */
      const char ** hcp;

      // if (!mparam.quad)
      if (!quad)
	{
	  hcp = triarules;
	  PrintMessage (3, "load internal triangle rules");
	}
      else
	{
	  hcp = quadrules;
	  PrintMessage (3, "load internal quad rules");
	  // LoadRules ("rules/quad.rls");
	}

	  PrintMessage (3, "Meshing2 :: LoadRules, testline 1");

      size_t len = 0;
      while (*hcp)
	{
	  //	  (*testout) << "POS2 *hcp " << *hcp << endl;
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 2");

	  len += strlen (*hcp);
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 3");

	  hcp++;
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 4");

	}
      //tr1 = new char[len+1];
      //tr1[0] = 0;
      tr1.reserve(len+1);
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 5");


      // if (!mparam.quad)
      if (!quad)
	hcp = triarules;
      else
	hcp = quadrules;
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 56");


      //char * tt1 = tr1;
      while (*hcp)
	{
	  //strcat (tt1, *hcp);
	  //tt1 += strlen (*hcp);
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 7");

	  tr1.append(*hcp);
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 8");

	  hcp++;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 9");

	}
      
#ifdef WIN32
      // VC++ 2005 workaround
	  for(string::size_type i=0; i<tr1.size(); i++)
	if(tr1[i] == ',')
	{
			  PrintMessage (3, "Meshing2 :: LoadRules, testline 10");

	  tr1[i] = ':';
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 11");

	}
#endif
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 12", tr1);

      ist = new istringstream (tr1);
    }

	  PrintMessage (3, "Meshing2 :: LoadRules, testline 13");

  if (!ist->good())
    {
      cerr << "Rule description file " << filename << " not found" << endl;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 14");

      delete ist;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 15");

      exit (1);
    }

  Timer t("Parsing rules");
  t.Start();
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 16");

  while (!ist->eof())
    {
      buf[0] = 0;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 17");

      (*ist) >> buf;
	  PrintMessage (3, "Meshing2 :: LoadRules, testline 18");
	  PrintMessage (3, buf);

      if (strcmp (buf, "rule") == 0)
	{
	  //(*testout) << "found rule" << endl;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 19, found rule", buf);

	  netrule * rule = new netrule;
	  //(*testout) << "fr1" << endl;
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 20");
	
	  rule -> LoadRule(*ist);
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 21");

	  //(*testout) << "fr2" << endl;
	  
	  rules.Append (rule);
	  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 22");

	}
      //(*testout) << "loop" << endl;
    }
  //(*testout) << "POS3" << endl;
  t.Stop();
  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 23");

  delete ist;
  	  PrintMessage (3, "Meshing2 :: LoadRules, testline 24");

  //delete [] tr1;
}

}
