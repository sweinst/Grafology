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

<table align="center">
<tr>
    <th align="center">Initial graph</th>
    <th align="center">Resulting graph</th>
</tr>

<tr>
    <td>
    &nbsp;&nbsp;&nbsp;&nbsp;
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
    &nbsp;&nbsp;&nbsp;&nbsp;
    </td>
    <td>
    &nbsp;&nbsp;&nbsp;&nbsp;
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
    &nbsp;&nbsp;&nbsp;&nbsp;
    </td>
</tr>
</table>


## Requirements
- the graph must be directed
- the graph must be acyclic (no cycle allowed)

## Complexity
- In a dense graph (highly connected), the complexity is $\lVert V \rVert^3$
- But it tends to $\lVert V \rVert^2$, the sparser the graph is.
