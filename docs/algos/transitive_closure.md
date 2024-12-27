<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Transitive closure
This algorithm transforms an existing graph $G(V,E)$ by adding edges between node which are indirectly connected.

So int the resulting graph $\forall{i,j,k}$, if $\exists\,G(i,k)$ and $\exists\,G(k,j)$, the edge $G(i,j)$ will be added if it doesn't exist.

<table align="center" width="100%">
<tr width="50%">
    <th align="center" width="50%"> Initial graph</th>
    <th align="center" width="50%"> Resulting graph</th>
</tr>

<tr width="50%">
    <td width="50%">
<div>

```mermaid
graph LR;
    A --> B
    A --> C
    C --> D
    D --> E
    E --> F
```

</div>
    </td>
    <td width="50%">
        <div>

```mermaid
graph LR;
    A --> B
    A --> C
    A --> D
    A --> E
    A --> F
    C --> D
    C --> E
    C --> F
    D --> E
    D --> F
```

</div>
    </td>
</tr>
</table>


## Requirements
- the graph must be directed
- the graph must be acyclic (no cycle allowed)

## Complexity
- In a dense graph (highly connected), the complexity is $\lVert V \rVert^3$
- But it tends to $\lVert V \rVert^2$, the sparser the graph is.
