# Tabela
| Tid | Name      | Phone   | Position  | Salary | Hired |
|-----|-----------|---------|-----------|--------|-------|
| t0  | W. Jones  | 202-222 | Developer | 2000   | 2012  |
| t1  | B. Jones  | 202-222 | Developer | 3000   | 2010  |
| t2  | J. Miller | 202-333 | Developer | 4000   | 2010  |
| t3  | D. Miller | 202-333 | DBA       | 8000   | 2010  |
| t4  | W. Jones  | 202-555 | DBA       | 7000   | 2010  |
| t5  | W. Jones  | 202-222 | Developer | 1000   | 2012  |

Name: String
Phone: String
Position: String
Salary: Numerical
Hired: Numerical

# Regras dos predicados

- Atributos não numéricos formam predicados com os operadores {=, !=}
- Atributos numéricos formam predicados com os operadores {=, !=, <, <=, >, >=}
- Somente atributos do mesmo tipo são comparados, ou seja, não existe atributo numérico contra não numérico.

# Predicados da Tabela

| p1  | tx.Name =  Ty.Name         |
| p2  | tx.Name != Ty.Name         |

| p3  | tx.Phone =  Ty.Phone       |
| p4  | tx.Phone != Ty.Phone       |

| p5  | tx.Position =  ty.Position |
| p6  | tx.Position != ty.Position |

| p7  | tx.Salary =  ty.Salary     |
| p8  | tx.Salary != ty.Salary     |
| p9  | tx.Salary <  ty.Salary     |
| p10 | tx.Salary <= ty.Salary     |
| p11 | tx.Salary >  ty.Salary     |
| p12 | tx.Salary >= ty.Salary     |

| p13 | tx.Hired =  ty.Hired       |
| p14 | tx.Hired != ty.Hired       |
| p15 | tx.Hired <  ty.Hired       |
| p16 | tx.Hired <= ty.Hired       |
| p17 | tx.Hired >  ty.Hired       |
| p18 | tx.Hired >= ty.Hired       |

